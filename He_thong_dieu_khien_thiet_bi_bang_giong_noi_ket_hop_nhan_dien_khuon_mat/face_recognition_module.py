import time
import cv2
import numpy as np
import face_recognition
import os

from notification_module import send_notification_with_image

image_folder = r"H:\Nam3\AI-IOT\He_thong_dieu_khien_thiet_bi_bang_giong_noi_ket_hop_nhan_dien_khuon_mat\data\data_face"
known_face_encodings = []
known_face_names = []

for filename in os.listdir(image_folder):
    file_path = os.path.join(image_folder, filename)
    try:
        # Đọc ảnh với OpenCV
        known_image = cv2.imread(file_path, cv2.IMREAD_UNCHANGED)

        # Kiểm tra nếu ảnh không phải 8-bit
        if known_image is None:
            print(f"Lỗi khi đọc ảnh {filename}: Không thể mở tệp.")
            continue

        # Kiểm tra định dạng ảnh
        if known_image.dtype != np.uint8:
            print(f"Chuyển đổi {filename} sang 8-bit...")
            known_image = cv2.convertScaleAbs(known_image)

        # Nếu ảnh là Grayscale, chuyển sang RGB
        if len(known_image.shape) == 2:
            print(f"Chuyển đổi {filename} từ Grayscale sang RGB...")
            known_image = cv2.cvtColor(known_image, cv2.COLOR_GRAY2RGB)

        # Chuyển đổi định dạng để face_recognition hỗ trợ
        known_encoding = face_recognition.face_encodings(known_image)

        if known_encoding:
            known_face_encodings.append(known_encoding[0])
            known_face_names.append(os.path.splitext(filename)[0])  
    except Exception as e:
        print(f"Lỗi khi xử lý {filename}: {e}")

if not known_face_encodings:
    print("Không có khuôn mặt hợp lệ trong thư mục!")
    exit()

print(f"Tải thành công {len(known_face_encodings)} khuôn mặt.")


authorized = False
last_auth_time = 0

def recognize_face():
    global authorized, last_auth_time
    cap = cv2.VideoCapture(0)

    cap.set(cv2.CAP_PROP_FPS, 30)
    cap.set(cv2.CAP_PROP_AUTO_EXPOSURE, 0.25)

    while True:
        ret, frame = cap.read()
        if not ret:
            continue

        rgb_frame = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
        face_locations = face_recognition.face_locations(rgb_frame)
        face_encodings = face_recognition.face_encodings(rgb_frame, face_locations)

        for (top, right, bottom, left), face_encoding in zip(face_locations, face_encodings):
            face_distances = face_recognition.face_distance(known_face_encodings, face_encoding)
            best_match_index = np.argmin(face_distances)

            if face_distances[best_match_index] < 0.4:
                user_name = known_face_names[best_match_index]
                color = (0, 255, 0)
                label = f"{user_name} ({face_distances[best_match_index]:.2f})"
                authorized = True
                last_auth_time = time.time()

                # Vẽ khung nhận diện
                cv2.rectangle(frame, (left, top), (right, bottom), color, 2)
                cv2.putText(frame, label, (left, top - 10), cv2.FONT_HERSHEY_SIMPLEX, 0.6, color, 2)

                cv2.imshow("Camera", frame)
                cv2.waitKey(3000)  
                cap.release()
                cv2.destroyAllWindows()
                return True

            else:
                user_name = "unknown"
                color = (0, 0, 255)
                label = "unknown"

                if not os.path.exists("unknown_faces"):
                    os.makedirs("unknown_faces")
                timestamp = time.strftime("%Y%m%d_%H%M%S")
                unknown_image_path = f"unknown_faces/unknown_{timestamp}.jpg"
                cv2.imwrite(unknown_image_path, frame)
                send_notification_with_image("Phát hiện người lạ!", "Có người lạ trước cửa.", unknown_image_path)

            cv2.rectangle(frame, (left, top), (right, bottom), color, 2)
            cv2.putText(frame, label, (left, top - 10), cv2.FONT_HERSHEY_SIMPLEX, 0.6, color, 2)

        cv2.imshow("Camera", frame)
        if cv2.waitKey(1) & 0xFF == ord("q"):  
            break  

    cap.release()
    cv2.destroyAllWindows()
    return False  




if __name__ == "__main__":
    print("Đang quét khuôn mặt...")
    recognize_face()

