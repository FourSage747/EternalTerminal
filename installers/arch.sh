#!/bin/bash
RED="\e[31m"
BLUE="\e[34m"
GREEN="\e[32m"
RESET="\e[0m"
set -e

if ! command -v pacman >/dev/null 2>&1; then
    echo -e "${RED}This installer requires Arch Linux.${RESET}"
    exit 1
fi

echo -e "${BLUE}Installing Arch packages...${RESET}"
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
source "$SCRIPT_DIR/packages.conf"

sudo pacman -Sy --needed --noconfirm \
    "${PACKAGES[@]}"
echo -e "${GREEN}Arch packages installed successfully.${RESET}"
