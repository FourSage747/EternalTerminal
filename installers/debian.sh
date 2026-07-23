#!/bin/bash
RED="\e[31m"
BLUE="\e[34m"
GREEN="\e[32m"
RESET="\e[0m"
set -e

if ! command -v apt >/dev/null 2>&1; then
    echo -e "${RED}This installer requires Debian Linux.${RESET}"
    exit 1
fi

echo -e "${BLUE}Installing packages for Debian-based systems...${RESET}"

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
source "$SCRIPT_DIR/packages.conf"

sudo apt update

sudo apt install -y \
    "${PACKAGES[@]}"


echo -e "${GREEN}Debian packages installed successfully.${RESET}"
