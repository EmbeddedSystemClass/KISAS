from gpiozero import LightSensor
sensor = LightSensor(23)

while True:
    sensor.wait_for_light()
    print("1")
    sensor.wait_for_dark()
    print("0")
    