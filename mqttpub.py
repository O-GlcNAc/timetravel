import pymysql
import paho.mqtt.client as mqtt
import time

# MQTT Broker configuration
broker_address = '127.0.0.1'
broker_port = 1883
topic = 'my/topic'

# MariaDB 연결 설정
conn = pymysql.connect(
    host='localhost',
    user='scott',
    password='tiger',
    database='exam'
)

cursor = conn.cursor()

# SensorData 테이블에서 가장 최신의 데이터 가져오기
query = """
    SELECT sensor_id, temperature, humidity, illuminance, timestamp
    FROM SensorData
    ORDER BY timestamp DESC
    LIMIT 1
"""

cursor.execute(query)
latest_data = cursor.fetchone()

# 만약 latest_data가 존재하면 MQTT 브로커로 전송
if latest_data:
    client = mqtt.Client()
    client.connect(broker_address, broker_port)
    
    sensor_id, temperature, humidity, illuminance, timestamp = latest_data
    message = f"sensor_id: {sensor_id}, temperature: {temperature}, humidity: {humidity}, illuminance: {illuminance}, timestamp: {timestamp}"
    
    client.publish(topic, message)
    print(f"Published: {message}")

conn.close()
