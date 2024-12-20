<div align="center">

# 🗂️ File Explorer

<h4>A modern file explorer built with C89 and vanilla JavaScript, featuring a custom template engine.</h4>

[Features](#-features) •
[Installation](#-installation) •
[Usage](#-usage) •
[Contributing](#-contributing) •
[License](#-license)

[![GitHub stars](https://img.shields.io/github/stars/AlphaTechnolog/file-explorer.svg?style=social&label=Star)](https://github.com/AlphaTechnolog/file-explorer)
[![GitHub forks](https://img.shields.io/github/forks/AlphaTechnolog/file-explorer.svg?style=social&label=Fork)](https://github.com/AlphaTechnolog/file-explorer/fork)
[![GitHub watchers](https://img.shields.io/github/watchers/AlphaTechnolog/file-explorer.svg?style=social&label=Watch)](https://github.com/AlphaTechnolog/file-explorer)

[![License](https://img.shields.io/github/license/AlphaTechnolog/file-explorer.svg)](https://github.com/AlphaTechnolog/file-explorer/blob/main/LICENSE)
[![Issues](https://img.shields.io/github/issues/AlphaTechnolog/file-explorer.svg)](https://github.com/AlphaTechnolog/file-explorer/issues)
[![Pull Requests](https://img.shields.io/github/issues-pr/AlphaTechnolog/file-explorer.svg)](https://github.com/AlphaTechnolog/file-explorer/pulls)

</div>

<details>
<summary><kbd>Table of contents</kbd></summary>

#### TOC

* [🌟 Project Overview](#-project-overview)
* [✨ Features](#-features)
* [🚀 Getting Started](#-getting-started)
  - [📋 Prerequisites](#-prerequisites)
  - [⚙️ Installation](#️-installation)
* [🖥️ Usage](#️-usage)
* [🛠️ Development](#️-development)
  - [🧪 Running Tests](#-running-tests)
* [🤝 Contributing](#-contributing)
* [📜 License](#-license)
* [👏 Acknowledgments](#-acknowledgments)
* [❓ FAQ](#-faq)
* [🔮 Roadmap](#-roadmap)
* [📊 Project Stats](#-project-stats)
* [🌐 Localization](#-localization)
* [🔒 Security](#-security)
* [💼 Support](#-support)
* [🔗 Related Projects](#-related-projects)
* [📣 Spread the Word](#-spread-the-word)
* [👤 Author](#-author)
* [🏆 Contributors](#-contributors)
* [📝 Changelog](#-changelog)

####

<br/>

</details>

## 🌟 Project Overview

File Explorer is a modern, lightweight file management application built with C89 and featuring a vanilla JavaScript frontend with a custom template engine. It aims to provide a seamless and efficient file browsing experience while maintaining a small footprint and high performance.

## ✨ Features

* 🚀 **Lightning-fast performance** with C89 backend
* 🖥️ **Intuitive UI** powered by vanilla JavaScript
* 🛠️ **Custom template engine** for efficient rendering
* 📁 **Multi-platform support** (Linux, macOS)
* 🔍 **Advanced search capabilities**
* 🎨 **Customizable themes**

## 🚀 Getting Started

### 📋 Prerequisites

Before you begin, ensure you have met the following requirements:

* C compiler (gcc or clang)
* Node.js (for frontend development)
* libgtk-3-dev
* libwebkit2gtk-4.0-dev (Linux) or WebKit framework (macOS)
* pkg-config
* make

### ⚙️ Installation

1. **Clone the repository:**

```sh
git clone https://github.com/AlphaTechnolog/file-explorer.git
cd file-explorer
```

2. **Initialize submodules:**

```shellscript
git submodule update --init --recursive
```


3. **Install dependencies:**

For Ubuntu/Debian:

```shellscript
sudo apt install libgtk-3-dev libwebkit2gtk-4.0-dev
```

For macOS:

```shellscript
brew install gtk+3
```

> [!NOTE]
> For Arch Linux users, install `webkit2gtk` instead of `webkitgtk-6.0`.

4. **Build the project:**

```shellscript
make
```

After successful compilation, you'll find the `file-explorer` binary in the project root.

## 🖥️ Usage

After successful compilation, you'll find the `file-explorer` binary in the project root. To launch File Explorer, run the following command from the project directory:

```sh
./file-explorer
```

This command will open the File Explorer application in your terminal.

## 🤝 Contributing

We welcome contributions to File Explorer! Here's how you can help:

1. Fork the repository
2. Create your feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

## 📜 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 👏 Acknowledgments

* [GTK](https://www.gtk.org/) for the GUI toolkit
* [WebKit](https://webkit.org/) for web content rendering
* All our amazing [contributors](#-contributors)

## ❓ FAQ

<details>
<summary><strong>Q: How does File Explorer compare to native file managers?</strong></summary>A: File Explorer aims to provide a lightweight, cross-platform alternative with a focus on performance and extensibility. While it may not have all the features of native file managers, it offers a customizable experience that can be tailored to specific needs.

</details><details>
<summary><strong>Q: Can I use File Explorer on Windows?</strong></summary>A: Currently, File Explorer supports Linux and macOS. Windows support is on our roadmap and we welcome contributions to make it happen!

</details>

## 🔮 Roadmap

* [ ] Windows support
* [ ] Sidebar with pinned places and favorites
* [ ] Context menu for files/folders
* [ ] Plugin system for extensibility
* [ ] Cloud storage integration

## 📊 Project Stats

## 🌐 Localization

File Explorer currently supports just English. Contributors are welcome to help with more languages!

## 🔒 Security

We take security seriously. If you discover a security vulnerability, please send an e-mail to [gfranklings@gmail.com](mailto:gfranklings@gmail.com).

## 💼 Support

For support, please open an issue on GitHub.

[comment]: <> (or join our [community chat]https://discord.gg/fileexplorer.)

## 🔗 Related Projects

* [Harakara Terminal](https://github.com/alphatechnolog/harakara-terminal)

## 📣 Spread the Word

Love File Explorer? Let the world know!

## 👤 Author

**AlphaTechnolog**

* GitHub: [@AlphaTechnolog](https://github.com/AlphaTechnolog)
* Twitter: [@alpha_technolog](https://twitter.com/alpha_technolog)

## 🏆 Contributors

Thanks to these wonderful people:

<a href="https://github.com/AlphaTechnolog/file-explorer/graphs/contributors">
  <img src="https://contrib.rocks/image?repo=AlphaTechnolog/file-explorer" />
</a>

---

<p align="center">
  Made with ❤️ by <a href="https://github.com/AlphaTechnolog">AlphaTechnolog</a>
</p>
