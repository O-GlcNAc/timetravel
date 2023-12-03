from flask import Flask, render_template
from flask_socketio import SocketIO, emit
import pymysql
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
from random import randrange
from sqlalchemy import create_engine
import paho.mqtt.client as mqtt


broker_address = '127.0.0.1'
broker_port = 1883
topic = 'my/topic'
received_data = []

app = Flask(__name__)
socketio = SocketIO(app, cors_allowed_origins="*")

def on_connect(client, userdata, flags, rc):
    print("Connected to MQTT broker")
    client.subscribe(topic)

def on_message(client, userdata, msg):
    print(f"Received: {msg.payload.decode()}")
    received_data.append(msg.payload.decode())
    if len(received_data) == 50:  # If 50 messages received
        process_data()

def process_data():
    # Process received_data (parse, convert to DataFrame, etc.)
    # Assuming received_data format: "sensor_id: ..., temperature: ..., humidity: ..., illuminance: ..., timestamp: ..."
    # Example parsing logic:
    sensor_ids, temperatures, humidities, illuminances, timestamps = [], [], [], [], []
    for data in received_data:
        parts = data.split(', ')
        sensor_ids.append(parts[0].split(': ')[1])
        temperatures.append(float(parts[1].split(': ')[1]))
        humidities.append(float(parts[2].split(': ')[1]))
        illuminances.append(float(parts[3].split(': ')[1]))
        timestamps.append(parts[4].split(': ')[1])

    df = pd.DataFrame({
        'sensor_id': sensor_ids,
        'temperature': temperatures,
        'humidity': humidities,
        'illuminance': illuminances,
        'timestamp': timestamps
    })

    generate_plot(df)

def generate_plot(df):
    plot = df.plot(use_index=True, y=["temperature", "humidity", "illuminance"], kind="line", figsize=(10, 5)).legend(loc='upper left')
    plt.savefig('static/plot.png')
    plt.close()
    emit('update_plot', 'plot.png')

client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message
client.connect(broker_address, broker_port)
client.loop_start()

@app.route('/')
def index():
    return render_template("index_plot.html")

@socketio.on('get_plot')
def handle_get_plot():
    # Assuming you have a function to retrieve sensor data from the processed MQTT messages
    sensor_data = get_sensor_data()
    if sensor_data is not None:
        generate_plot(sensor_data)

if __name__ == '__main__':
    socketio.run(app, port=5001)
