#!/bin/bash

# Запускаем бота-регистратора
echo "[SYSTEM] Starting Python Register Bot..."
# python3 /app/bot_handlers/register.py &

# Ждем немного
sleep 2

# Запускаем агент
echo "[SYSTEM] Starting Aura Agent..."
aura-agent
