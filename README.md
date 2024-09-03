# Webview Notify

Example app that sends notifications using html, tailwind and js for the frontend but uses c89 for the backend.

This is built as a personal challenge where i wanted to write a desktop application using bare C for the backend
but also with the idea of writing a template system, or at least a very barebones implementation of somewhat similar.

So the result is this repository, this includes a sample application which given a text input and a message,
you can then press a button and it will send a notification with the previously entered input message.

## Technologies used

- C for the desktop app code
- HTML/Tailwind for the frontend and js for the interactivity in the frontend
- Webview for the windows abstractions and the frontend rendering aswell, useful for communication between c and js aswell.

## Getting started

To get started just clone and build the application, you will probably have to install the next dependencies first
though. Works only on linux at the moment.

- libnotify
- pkg-config
- webkit-gtk
- gtk4
- cmake
- make
- gcc
- nodejs, npm
- git

> [!NOTE]
> Windows support will be ~~insane~~ interesting to add though...

Then you can build it this way.

```sh
git clone --recursive https://github.com/AlphaTechnolog/webview-templates-notify
cd webview-templates-notify
make
npm install
```

> [!WARNING]
> In the first make run, you wont be able to use the `-j` flag since it will download and build webview.
> Anyways you can start using it after the lib is built, even after make clean, to make clean the libs too, do make clean-libs

After this, you can run `./webview-example`.

## Template system example

You can see the frontend code at [src/resources](./src/resources) and the template consists in an index.html
file which contains directives similar to this one:

> [!NOTE]
> Atm only supporting `INCLUDE` but the code supports registering more directives with custom code.

```html
<!DOCTYPE html>
<html lang="en">
<head>
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <title>Example Page</title>
        <style>
                ~~INCLUDE:styles/global.css,styles/main.css~~
        </style>
</head>
<body>
        <main>
                ~~INCLUDE:components/main-content.html~~
        </main>

        <script>
                ~~INCLUDE:scripts/general-behavior.js,scripts/main.js~~
        </script>
</body>
</html>
```

This will replace every include directive with the content inside of

- `styles/global.css`
- `styles/main.css`
- `components/main-content.html`
- `scripts/general-behavior.js`
- `scripts/main.js`

This allows splitting the source code when still making webview believe it's anyways only one file.

## Why?

Idk this was fun to build and also frustrating but served me to learn about C in general and about webview which seems cool for some little desktop apps which involves heavy use of system libraries, something like an app which controls the system behavior or somewhat similar idk.
