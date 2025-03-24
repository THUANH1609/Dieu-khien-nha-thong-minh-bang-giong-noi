from face_recognition_module import recognize_face
from voice_control_module import get_audio
from esp32_control import send_command

list_command = ['bật quạt', 'tắt quạt', 'bật đèn phòng ngủ', 'tắt đèn phòng ngủ',
                'bật đèn phòng khách', 'tắt đèn phòng khách','mở cửa sổ', 'đóng cửa sổ', 'mở nhạc', 'tắt nhạc']

if __name__ == "__main__":
    print("Đang quét khuôn mặt...")
    if recognize_face():
        print("Xác thực thành công! Bạn có thể điều khiển thiết bị.")
    else:
        print("Xác thực thất bại! Thoát chương trình.")
        exit()  

    while True:
        print("Bạn muốn tôi làm gì?")
        command = get_audio()

        if command in list_command:
            send_command(command)
            print(f"Đã thực hiện lệnh: {command}")
        else:
            print("Lệnh không hợp lệ! Hãy thử lại.")
