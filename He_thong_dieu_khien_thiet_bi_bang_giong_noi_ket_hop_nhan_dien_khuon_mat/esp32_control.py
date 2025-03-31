import requests

ESP32_IP = "http://192.168.162.5"

def send_command(command):
    url = f"{ESP32_IP}/control"
    data = {}

    command_map = {
        "bật quạt":  ("quat", 1),
        "tắt quạt":  ("quat", 0),
        "bật đèn phòng ngủ":   ("den 1", 1),
        "tắt đèn phòng ngủ":   ("den 1", 0),
        "bật đèn phòng khách":   ("den 3", 1),
        "tắt đèn phòng khách":   ("den 3", 0),
        "mở cửa sổ":    ("cua", 1),
        "đóng cửa sổ":  ("cua", 0),
        "mở nhạc":   ("nhac", 1),
        "tắt nhạc":  ("nhac", 0)
    }

    device, state = command_map.get(command, (None, None))
    if device is None:
        print("Lệnh không hợp lệ!")
        return

    data = {"device": device, "state": state}
    try:
        response = requests.post(url, data=data)
        print(f"Gửi lệnh: {data}")
        print("Phản hồi từ ESP32:", response.text)
    except requests.exceptions.RequestException as e:
        print("Lỗi kết nối đến ESP32:", e)
