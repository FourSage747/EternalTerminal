#!/bin/bash

# Шлях до папки з твоїми картинками
ART_DIR="$HOME/.config/kitty/arts"

# Збираємо всі файли з папки у масив
# shopt -s nullglob запобігає помилкам, якщо папка раптом порожня
shopt -s nullglob
FILES=("$ART_DIR"/*)

# Перевіряємо, чи є взагалі картинки в папці
if [ ${#FILES[@]} -eq 0 ]; then
    echo "Put pictures in folder $ART_DIR!"
    while true; do sleep 1; done
fi

# Вибираємо випадкову картинку за допомогою вбудованого генератора $RANDOM
RANDOM_INDEX=$((RANDOM % ${#FILES[@]}))
IMAGE_FILE="${FILES[$RANDOM_INDEX]}"

draw_image() {
    clear 
    cols=$(tput cols)
    lines=$(tput lines)
    
    # Викликаємо chafa з нашими улюбленими налаштуваннями шрифту Брайля
    # (Можеш замінити braille на block,stipple,space, якщо тобі той варіант сподобався більше)
    chafa -c full -f symbols --size="${cols}x${lines}" --symbols braille --fg-only "$IMAGE_FILE"
   # chafa -c full -f symbols --size="${cols}x${lines}" --symbols ascii --fg-only "$IMAGE_FILE"
}

# Перехоплюємо сигнал зміни розміру вікна
trap draw_image SIGWINCH

# Малюємо перший раз
draw_image

# Безкінечний цикл, щоб вікно не закривалося
while true; do sleep 1; done
