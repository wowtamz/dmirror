async function getLatestRelease(owner, repo) {
  const response = await fetch(
    `https://api.github.com/repos/${owner}/${repo}/releases/latest`
  );

  const release = await response.json();
  return release;
}

async function main() {

  const release = await getLatestRelease("wowtamz", "dmirror");

  const osDict = {
    'win64': 'dl_win64',
    'macOS': 'dl_macos',
    'linux-x86_64.AppImage': 'dl_appimage',
    'linux_x64.deb': 'dl_deb'
  };

  const downloadTitle = document.getElementById("dl_title");
  downloadTitle.innerHTML = downloadTitle.innerHTML + " " + release.tag_name;
  console.log(release.tag_name);
    
  release.assets.forEach(asset => {

    for (const key in osDict) {
      if (asset.name.includes(key)) {
        const link = document.getElementById(osDict[key]);
        link.href = asset.browser_download_url;
      }
    }
  });

  const modal = document.getElementById("dl_modal");

  document.getElementById("openModal").onclick = () => {
    modal.classList.add("show");
  };

document.getElementById("closeModal").onclick = () => {
    modal.classList.remove("show");
  };
}

main();
