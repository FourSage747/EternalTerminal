#!/bin/bash

set -e

echo "================================="
echo " Eternal Terminal Installer"
echo "================================="

# Check Arch
if ! command -v pacman &>/dev/null; then
    echo "Error: This installer supports Arch Linux only."
    exit 1
fi


echo "[1/5] Installing packages..."

sudo pacman -S --needed --noconfirm \
    kitty \
    zsh \
    starship \
    eza \
    chafa \
    zsh-syntax-highlighting \
    otf-comicshanns-nerd


echo "[2/5] Creating backups..."

BACKUP_DIR="$HOME/.config/eternal-terminal-backup"

mkdir -p "$BACKUP_DIR"


if [ -d "$HOME/.config/kitty" ]; then
    cp -r "$HOME/.config/kitty" "$BACKUP_DIR/kitty"
fi


if [ -f "$HOME/.config/starship.toml" ]; then
    cp "$HOME/.config/starship.toml" "$BACKUP_DIR/starship.toml"
fi


if [ -f "$HOME/.zshrc" ]; then
    cp "$HOME/.zshrc" "$BACKUP_DIR/.zshrc"
fi


echo "[3/5] Installing Kitty configuration..."

mkdir -p "$HOME/.config/kitty"

cp -r kitty/* "$HOME/.config/kitty/"


echo "[4/5] Installing Starship..."

cp starship/starship.toml \
    "$HOME/.config/starship.toml"


echo "[5/5] Installing ZSH configuration..."

cp zsh/.zshrc "$HOME/.zshrc"


chmod +x "$HOME/.config/kitty/draw_art.sh"


echo ""
echo "================================="
echo " Installation completed!"
echo "================================="
echo ""
echo "Restart your terminal or run:"
echo "exec zsh"
echo ""
echo "Enjoy Eternal Terminal!"
