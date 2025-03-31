import speech_recognition as sr

# Nhận lệnh giọng nói
def get_audio():
    recognizer = sr.Recognizer()
    with sr.Microphone() as source:
        print(" Bạn: ", end="")
        audio = recognizer.listen(source, phrase_time_limit=5)
    
    try:
        text = recognizer.recognize_google(audio, language="vi-VN")
        print(text)
        return text.lower()
    except:
        print("Không nghe được...")
        return None
