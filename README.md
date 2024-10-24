# File Explorer

A file explorer built with bare c89 and a vanilla-javascript based frontend with a custom template engine :)

## Installation

At the moment we can just build it from source, and you'll need to install some dependencies before.

```sh
apt install libgtk-3-dev libwebkit2gtk-4.0-dev
# You'll also need to install nodejs either by using nvm or some other way.
```

> [!NOTE]
> This project got mac support, but if building for mac, WebKit framework should already be available and ready to be linked with, you would have to probably install `gtk+3` with brew tho.

> You'll also probably need to install pkg-config, get a compiler, either it's clang or gcc, and some build-essentials. NOTE: For arch users you will need the webkit2gtk pkg, not webkitgtk-6.0

After so, you can clone the project with all the submodules and then issue `make`:

```sh
git clone https://github.com/AlphaTechnolog/file-explorer.git file-explorer
cd file-explorer
git submodule update --init --recursive
make
```

Then, you'll be able to find the `file-explorer` binary in the root of the project.

## Missing features

- [ ] Sidebar with pinned places and favorites such as desktop, pictures, etc.
- [ ] Context menu for files/folders
- [ ] Open files with double click or single click idk
