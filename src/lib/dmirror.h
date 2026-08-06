// dmirror.h
#pragma once

#include <filesystem>
#include <fstream>


enum class CopyResult
{
    Success,
    Failed,
    Cancelled
};

std::string dmirror_get_file_sig(const std::string& file_path)
{

    if (!std::filesystem::exists(file_path)) {
        return "";
    }

    std::error_code ec;
    uintmax_t size = std::filesystem::file_size(file_path, ec);
    if (ec) {
        throw std::runtime_error("Error occurred while getting file size");
    }

    std::string file_size = std::to_string(size);

    std::filesystem::file_time_type last_write_time = std::filesystem::last_write_time(file_path, ec);
    if (ec) {
        throw std::runtime_error("Error occurred while getting file's last write time");
    }

    auto ticks = std::chrono::duration_cast<std::chrono::nanoseconds>(last_write_time.time_since_epoch()).count();;

    return file_size + "_" + std::to_string(ticks);
}

bool dmirror_cmp_files(const std::string& file_x_path, const std::string& file_y_path)
{
    return dmirror_get_file_sig(file_x_path) == dmirror_get_file_sig(file_y_path);
}

bool dmirror_copy_file(const std::string& src_path, const std::string& dst_path)
{
    if (dmirror_cmp_files(src_path, dst_path)) {
        std::cout << "Files are identical, skipping copy: " << src_path << " -> " << dst_path << std::endl;
        return true;
    }

    bool success = std::filesystem::copy_file(src_path, dst_path, std::filesystem::copy_options::overwrite_existing);
    if (!success) {
        std::cerr << "Failed to copy file from " << src_path << " to " << dst_path << std::endl;
        return false;
    }

    // Preserve the last write time of the source file for signature comparison
    std::error_code ec;
    std::filesystem::last_write_time(dst_path, std::filesystem::last_write_time(src_path));

    if (ec) {
        std::cerr << "Failed to preserve last write time for " << dst_path << ": " << ec.message() << std::endl;
        return false;
    }

    std::cout << "Successfully copied file " << dst_path << std::endl;

    return true;
}


CopyResult dmirror_copy_dir(const std::string& src_path, const std::string& dst_path, std::function<void(int current, int total)> progress_callback = nullptr, std::function<bool()>is_cancelled = nullptr)
{
    std::filesystem::create_directory(dst_path);

    int current = 0;
    int total = 0;

    for (const auto& element : std::filesystem::recursive_directory_iterator(src_path)) {
        if (element.is_regular_file()) {
            total++;
        }
    }

    if (progress_callback != nullptr) {
        progress_callback(current, total);
    }

    for (const auto& element : std::filesystem::recursive_directory_iterator(src_path)) {

        if (is_cancelled != nullptr && is_cancelled()) {
            return CopyResult::Cancelled;
        }

        std::string relative_path = std::filesystem::relative(element.path(), src_path).string();

        if (element.is_directory()) {
            std::filesystem::create_directory(dst_path + "/" + relative_path);
        } else if (element.is_regular_file()) {
            int try_count = 0;
            while (try_count < 3) {
                bool success = dmirror_copy_file(element.path().string(), dst_path + "/" + relative_path);
                if (success) {
                    current++;
                    if (progress_callback != nullptr) {
                        progress_callback(current, total);
                    }
                    break;
                } else {
                    std::cerr << "Retrying copy for file: " << element.path() << std::endl;
                    try_count++;
                }
            }
            if (try_count == 3) {
                std::cerr << "Failed to copy file after 3 attempts: " << element.path() << std::endl;
                return CopyResult::Failed;
            }
        }
    }

    return CopyResult::Success;
}

