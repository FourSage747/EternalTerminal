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

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
source "$SCRIPT_DIR/packages.conf"

sudo dnf install -y \
    "${PACKAGES[@]}"


echo -e "${GREEN}Fedora packages installed successfully.${RESET}"
