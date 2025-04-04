import socket
import mediapipe as mp
import cv2

# Initialize MediaPipe Hands
mp_hands = mp.solutions.hands
mp_drawing = mp.solutions.drawing_utils


hands = mp_hands.Hands(min_detection_confidence=0.7, min_tracking_confidence=0.7)

# Set up socket server
server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_socket.bind(("127.0.0.1", 65432))
server_socket.listen()

print("Waiting for connection...")
conn, addr = server_socket.accept()
print(f"Connected by {addr}")

cap = cv2.VideoCapture(0)

while True:
    ret, frame = cap.read()
    if not ret:
        break

    frame = cv2.flip(frame, 1)  # Flip the frame horizontally
    rgb_frame = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)  # Convert to RGB for MediaPipe
    result = hands.process(rgb_frame)

    gesture = "stop"  # Default gesture

    if result.multi_hand_landmarks:
        for hand_landmarks in result.multi_hand_landmarks:
            # Draw hand landmarks on the frame
            mp_drawing.draw_landmarks(frame, hand_landmarks, mp_hands.HAND_CONNECTIONS)

            # Example: Check if index finger is raised
            index_tip = hand_landmarks.landmark[mp_hands.HandLandmark.INDEX_FINGER_TIP]
            index_dip = hand_landmarks.landmark[mp_hands.HandLandmark.INDEX_FINGER_DIP]

            if index_tip.y < index_dip.y:
                gesture = "forward"
            elif index_tip.x < hand_landmarks.landmark[mp_hands.HandLandmark.WRIST].x:
                gesture = "left"
            elif index_tip.x > hand_landmarks.landmark[mp_hands.HandLandmark.WRIST].x:
                gesture = "right"

    # Display the gesture on the frame
    cv2.putText(frame, f"Gesture: {gesture}", (10, 30), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0), 2, cv2.LINE_AA)

    # Send gesture to the connected client
    if conn.fileno() != -1:  # Ensure the socket is still open
        conn.sendall((gesture + "\n").encode())  # Append newline as delimiter
    else:
        print("Socket is closed.")

    # Show the frame with landmarks and gesture classification
    cv2.imshow("Hand Gesture Recognition", frame)

    # Break the loop if 'Esc' key is pressed
    if cv2.waitKey(5) & 0xFF == 27:
        break

cap.release()
conn.close()
server_socket.close()
