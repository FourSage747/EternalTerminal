#!/bin/bash
RED="\e[31m"
BLUE="\e[34m"
GREEN="\e[32m"
RESET="\e[0m"
set -e

if ! command -v dnf >/dev/null 2>&1; then
    echo -e "${RED}This installer requires Fedora Linux.${RESET}"
    exit 1
fi

echo -e "${BLUE}Installing packages for Fedora-based systems...${RESET}"

PACKAGES=(
    kitty
    zsh
    starship
    eza
    chafa
    zsh-syntax-highlighting
)

sudo dnf install -y \
    "${PACKAGES[@]}"


echo -e "${GREEN}Fedora packages installed successfully.${RESET}"
