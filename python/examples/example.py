import optical_flow
import cv2
import time

# Create the optical flow object
flow = optical_flow.OpticalFlow(1.3962634, -1, 0.0)

# Open the camera
cap = cv2.VideoCapture(0)

while True:
    ret, frame = cap.read()
    # Convert to grayscale
    frame = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    
    # Make sure the image is 64x64 for the optical flow
    # Scale and crop if necessary to make sure it is 64x64
    width = frame.shape[1]
    height = frame.shape[0]
    min_dim = min(width, height)
    frame = frame[int(height/2 - min_dim/2):int(height/2 + min_dim/2), int(width/2 - min_dim/2):int(width/2 + min_dim/2)]
    frame = cv2.resize(frame, (64, 64))
    cv2.imshow('frame', frame)

    # Run the optical flow
    msg = flow.feed(frame, time.time())

    # Print the optical flow message
    if msg is not None and msg.quality > 0:
        print(msg)

    # Quit if q is pressed
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break
