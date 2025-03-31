import requests

PUSHOVER_USER_KEY = "u4uvq5mjgm8didmaa9rwo74hr24bxp"
PUSHOVER_API_TOKEN = "ad63f92ts62ufy53yvpg9yy8ajzj2q"

def send_notification_with_image(title, message, image_path):
    url = "https://api.pushover.net/1/messages.json"
    
    with open(image_path, "rb") as image_file:
        files = {"attachment": image_file}
        data = {
            "token": PUSHOVER_API_TOKEN,
            "user": PUSHOVER_USER_KEY,
            "message": message,
            "title": title,
        }

        try:
            response = requests.post(url, data=data, files=files)
            print(f"Thông báo gửi thành công! Trạng thái: {response.status_code}")
        except requests.exceptions.RequestException as e:
            print(f" Lỗi khi gửi thông báo: {e}")
