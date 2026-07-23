#!/bin/bash

GREEN="\e[32m"
RED="\e[31m"
YELLOW="\e[33m"
BLUE="\e[34m"
RESET="\e[0m"
set -e

echo "================================="
echo -e " ${BLUE}Eternal Terminal Installer${RESET}"
echo "================================="


echo -e "${GREEN}[1/6] Installing packages...${RESET}"
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

if command -v pacman >/dev/null 2>&1; then
    echo "===Detected: pacman==="
    bash "$SCRIPT_DIR/installers/arch.sh"

elif command -v apt >/dev/null 2>&1; then
    echo "===Detected: apt==="
    bash "$SCRIPT_DIR/installers/debian.sh"

elif command -v dnf >/dev/null 2>&1; then
    echo "===Detected: dnf==="
    bash "$SCRIPT_DIR/installers/fedora.sh"

elif command -v zypper >/dev/null 2>&1; then
    echo "===Detected: zypper==="
    bash "$SCRIPT_DIR/installers/opensuse.sh"

else
    echo -e "${RED}===Unsupported Linux distribution===${RESET}"
    exit 1
fi


echo -e "${GREEN}[2/6] Creating backups...${RESET}"

BACKUP_DIR="$HOME/.config/your-terminal-backup"

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

echo -e "${GREEN}[3/6] Installing fonts...${RESET}"

bash "$SCRIPT_DIR/fonts/install-fonts.sh"


echo -e "${GREEN}[4/6] Installing Kitty configuration...${RESET}"

mkdir -p "$HOME/.config/kitty"

cp -r kitty/* "$HOME/.config/kitty/"


echo -e "${GREEN}[5/6] Installing Starship...${RESET}"

cp starship/starship.toml \
    "$HOME/.config/starship.toml"


echo -e "${GREEN}[6/6] Installing ZSH configuration...${RESET}"

cp zsh/.zshrc "$HOME/.zshrc"


chmod +x "$HOME/.config/kitty/draw_art.sh"


echo ""
echo "================================="
echo -e "${GREEN} Installation completed!${RESET}"
echo "================================="
echo ""
echo -e "${BLUE}Restart your terminal or run:${RESET}"
echo "exec zsh"
echo ""
echo "Enjoy Eternal Terminal!"
echo -e "${YELLOW}If you didn't like it, you can always find your previos terminal version in ${BACKUP_DIR}${RESET}"
