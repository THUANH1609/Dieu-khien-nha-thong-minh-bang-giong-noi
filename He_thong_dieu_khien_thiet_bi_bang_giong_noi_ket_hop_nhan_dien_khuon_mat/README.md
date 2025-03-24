<h1 align="center">HỆ THỐNG ĐIỀU KHIỂN THIẾT BỊ NHÀ THÔNG MINH BẰNG NHẬN DIỆN KHUÔN MẶT VÀ GIỌNG NÓI </h1>
<div align="center">

<p align="center">
  <img src="image\logoDaiNam.png" alt="DaiNam University Logo" width="200"/>
  <img src="image\LogoAIoTLab.png" alt="AIoTLab Logo" width="170"/>
</p>

[![Made by AIoTLab](https://img.shields.io/badge/Made%20by%20AIoTLab-blue?style=for-the-badge)](https://www.facebook.com/DNUAIoTLab)
[![Fit DNU](https://img.shields.io/badge/Fit%20DNU-green?style=for-the-badge)](https://fitdnu.net/)
[![DaiNam University](https://img.shields.io/badge/DaiNam%20University-red?style=for-the-badge)](https://dainam.edu.vn)

</div>

---

## 🌟 Giới thiệu

- Nhận diện khuôn mặt: Dùng OpenCV và Face Recognition để quét, mã hóa và so sánh khuôn mặt. Nếu trùng khớp, cho phép điều khiển thiết bị, nếu không, chụp ảnh và gửi cảnh báo.
- Nhận diện giọng nói: Dùng Speech Recognition để chuyển giọng nói thành văn bản, kiểm tra lệnh và gửi tín hiệu điều khiển qua ESP32.
- Kết nối IoT: ESP32 kết nối thiết bị điều khiển đèn, quạt, cửa sổ, loa bằng giọng nói.

---

## ⚙ Hệ thống
<p align="center">
  <img src="image\hethong.png" alt="System Architecture" width="800"/>
</p>

---
## 📂 Cấu trúc dự án

📦 Project 
├── 📂arduino
| ├──esp32.ino       # Mã nguồn Arduino điều khiển các thiết bị
├── 📂 data 
│    ├── 📂 data_face            # Chứa dữ liệu khuôn mặt.
|    ├── 📂 unknown_faces      # Chứa dữ liệu khuôn mặt người lạ
├── esp32_control.py            # Điều khiển ESP32
├── face_recognition_module.py  # Module nhận diện khuôn mặt
├── main.py                     #  Tệp chính chạy chương trình.
├── notification_module.py      # Quản lý thông báo.
├── voice_control_module.py  # Điều khiển bằng giọng nói.

---
## 💻 Công nghệ sử dụng 
### Phần cứng


| **PHẦN CỨNG** | **SỬ DỤNG** |
|---------------------|-------------|
| ESP32             | Nhận tín hiệu điều khiển các thiết bị |
| DFPlayer          | Loa         |
| Động cơ servo     | Động cơ điều khiển cửa |
| Relay Module      | Điều khiển quạt |
| Đèn Led, Quạt        |                 |

### Phần mềm
| **PHẦN MỀM** | **SỬ DỤNG** |
|---------------------|-------------|
| PYTHON
| PUSHOVER| App nhận cảnh báo người lạ|
| Arduino IDE| Để nạp file .ino|

###  Các thư viện Python cần thiết
Cài đặt các thư viện cần thiết:

    pip install opencv-python face-recognition SpeechRecognition

---
## 🖇 Hướng dẫn cài đặt và chạy
1️⃣ Chuẩn bị phần cứng: Nạp arduino
- Mở file esp32.ino bằng Arduino IDE.
- Kết nối board Arduino với máy tính.
- Nạp (upload) mã nguồn lên board.
- Mở Serial Monitor hiện ESP32_IP 

2️⃣ Cài đặt PushOver

- Tải App PushOver và đăng nhập trên điện thoại(Androi/IOS)
- Vào https://pushover.net/ đăng nhập 
- Tạo Create an Application/API Token để lấy 'Your User Key' và 'API Token/Key'

3️⃣ Chạy chương trình

- Lấy địa chỉ IP ở Arduino IDE vào chương trình `esp32_control.py` 
- Lấy User Key và API Token/Key ở PushOver vào chương trình `notification_module.py`
- Chạy chương trình `main.py`

### 📑 Hướng dẫn sử dụng
1️⃣ Nhận diện khuôn mặt

- Khi người dùng chạy chương trình camera quét khuôn mặt trong phạm vi giám sát.
- So sánh khuôn mặt hiện tại vs khuôn mặt đã lưu
    
    - ✅ Đúng với khuôn mặt → Có thể điều khiển được các thiết bị bằng giọng nói.
    - ❌ Khuôn mặt không khớp → Chụp, lưu dữ liệu vào `unknown_faces` và thông báo đến điện thoại qua App PushOver.

2️⃣ Nhận giọng nói điều khiển thiết bị trong nhà

- Sử dụng các câu lệnh điều khiển các thiết bị
   
   - Quạt: 'bật quạt', 'tắt quạt'.
   - Đèn: 'bật đèn phòng ngủ', 'tắt đèn phòng ngủ','bật đèn phòng khách', 'tắt đèn phòng khách'.
   - Cửa sổ: 'mở cửa sổ', 'đóng cửa sổ'
   - Loa: 'mở nhạc', 'tắt nhạc'.

3️⃣ Xem thông báo cảnh báo người lạ

- Qua App PushOver xem được ảnh chụp người lạ kèm ngày tháng.
- Ảnh sẽ được lưu vào data/unknown_faces kèm thời gian.

--- 
## 📰 Poster

<p align="center">
  <img src="\image\Poster_CNTT06_NHOM9.png" alt="System Architecture" width="800"/>
</p>

---
## 🤝 Đóng góp
Các thành viên nhóm

- Nguyễn Thu Anh
- Nguyễn Thị Khuyên
- Hà Duy Dương
- Đỗ Văn Thuyên

© 2025 NHÓM 9, CNTT16-06, TRƯỜNG ĐẠI HỌC ĐẠI NAM