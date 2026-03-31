import json
import asyncio
from aiogram import Bot, Dispatcher, types, filters
from aiogram.types import ChatMemberStatus

TOKEN = "8494719218:AAFWPF4G5RzCDSYxfRPFrL9Zz_TT41JNJdE"
CHANNEL_ID = "@auralith_it" # ID канала (через @ или ID)
USER_FILE = "users.json"

bot = Bot(token=TOKEN)
dp = Dispatcher()

async def is_subscribed(user_id):
    try:
        member = await bot.get_chat_member(chat_id=CHANNEL_ID, user_id=user_id)
        # Проверяем, что юзер не вышел из канала и не забанен
        return member.status in [ChatMemberStatus.MEMBER, ChatMemberStatus.ADMINISTRATOR, ChatMemberStatus.CREATOR]
    except Exception as e:
        print(f"Ошибка проверки подписки: {e}")
        return False

def save_user(user_id):
    try:
        with open(USER_FILE, "r") as f:
            users = json.load(f)
    except (FileNotFoundError, json.JSONDecodeError):
        users = []

    if user_id not in users:
        users.append(user_id)
        with open(USER_FILE, "w") as f:
            json.dump(users, f)
        return True
    return False

@dp.message(filters.Command("start"))
async def start_handler(message: types.Message):
    user_id = message.chat.id
    
    # Проверяем подписку
    if await is_subscribed(user_id):
        if save_user(user_id):
            await message.answer(f"✅ Спасибо за подписку на {CHANNEL_ID}!\nТеперь вы будете получать уведомления Aura Agent.")
        else:
            await message.answer("🔔 Вы уже в списке получателей.")
    else:
        await message.answer(
            f"❌ Для получения уведомлений нужно подписаться на наш канал: {CHANNEL_ID}\n\n"
            "После подписки снова нажми /start"
        )

async def main():
    print("Регистратор с проверкой канала запущен...")
    await dp.start_polling(bot)

if __name__ == "__main__":
    asyncio.run(main())
