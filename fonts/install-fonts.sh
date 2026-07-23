#!/bin/bash

set -e

FONT_NAME="ComicShannsMono Nerd Font"

FONT_DIR="$HOME/.local/share/fonts"

echo "Installing ComicShannsMono Nerd Font..."

mkdir -p "$FONT_DIR"

cd /tmp


curl -L \
https://github.com/ryanoasis/nerd-fonts/releases/latest/download/ComicShannsMono.zip \
-o ComicShannsMono.zip


unzip -o ComicShannsMono.zip \
-d "$FONT_DIR/ComicShannsMono"


fc-cache -fv


echo "Font installed successfully."
