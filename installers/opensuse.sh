#!/bin/bash
RED="\e[31m"
BLUE="\e[34m"
GREEN="\e[32m"
RESET="\e[0m"
set -e

if ! command -v zypper >/dev/null 2>&1; then
    echo -e "${RED}This installer requires Suse Linux.${RESET}"
    exit 1
fi

echo -e "${BLUE}Installing packages for openSUSE...${RESET}"

PACKAGES=(
    kitty
    zsh
    starship
    eza
    chafa
    zsh-syntax-highlighting
)

sudo zypper install -y \
    "${PACKAGES[@]}"


echo -e "${GREEN}openSUSE packages installed successfully.${RESET}"
