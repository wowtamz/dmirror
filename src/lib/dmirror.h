// dmirror.h
#pragma once

#include <filesystem>
#include <fstream>

std::string dmirror_get_file_sig(std::string& file_path)
{
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

bool dmirror_cmp_files(std::string& file_x_path, std::string& file_y_path)
{
    return dmirror_get_file_sig(file_x_path) == dmirror_get_file_sig(file_y_path);
}

bool dmirror_copy_file(const std::string& src_path, const std::string& dest_path)
{
    bool success = std::filesystem::copy_file(src_path, dest_path, std::filesystem::copy_options::overwrite_existing);
    if (!success) {
        std::cerr << "Failed to copy file from " << src_path << " to " << dest_path << std::endl;
        return false;
    }

    // Preserve the last write time of the source file for signature comparison
    std::error_code ec;
    std::filesystem::last_write_time(dest_path, std::filesystem::last_write_time(src_path));

    if (ec) {
        std::cerr << "Failed to preserve last write time for " << dest_path << ": " << ec.message() << std::endl;
        return false;
    }

    return true;
}


bool dmirror_copy_dir(const std::string& src_path, const std::string& dest_path)
{
    // We assume that dest_path already exists and is a directory.
    std::filesystem::create_directory(dest_path);

    for (const auto& element : std::filesystem::recursive_directory_iterator(src_path)) {

        std::string relative_path = std::filesystem::relative(element.path(), src_path).string();

        if (element.is_directory()) {
            std::filesystem::create_directory(dest_path + "/" + relative_path);
        } else if (element.is_regular_file()) {
            int try_count = 0;
            while (try_count < 3) {
                bool success = dmirror_copy_file(element.path().string(), dest_path + "/" + relative_path);
                if (success) {
                    std::cout << "Successfully copied file " << element.path() << std::endl;
                    break;
                } else {
                    std::cerr << "Retrying copy for file: " << element.path() << std::endl;
                    try_count++;
                }
            }
            if (try_count == 3) {
                std::cerr << "Failed to copy file after 3 attempts: " << element.path() << std::endl;
                return false;
            }
        }
    }

    return true;
}

