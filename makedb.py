import pymysql

# MariaDB 연결 설정
conn = pymysql.connect(
    host='localhost',
    user='scott',
    password='tiger',
    database='exam'  # 이미 생성된 exam 데이터베이스 선택
)

# 커서 생성
cursor = conn.cursor()

# Sensors 테이블 생성
cursor.execute("""
    CREATE TABLE IF NOT EXISTS Sensors (
        id INTEGER AUTO_INCREMENT PRIMARY KEY,
        name VARCHAR(30),
        location VARCHAR(50)
    )
""")
print("Sensors 테이블이 생성되었습니다.")

# SensorData 테이블 생성
cursor.execute("""
    CREATE TABLE IF NOT EXISTS SensorData (
        id INTEGER AUTO_INCREMENT PRIMARY KEY,
        sensor_id INTEGER,
        reading FLOAT,
        timestamp TIMESTAMP,
        temperature FLOAT,
        humidity FLOAT,
        illuminance FLOAT,
        FOREIGN KEY (sensor_id) REFERENCES Sensors(id)
    )
""")
print("SensorData 테이블이 생성되었습니다.")

# Sensorstatus 테이블 생성
cursor.execute("""
    CREATE TABLE IF NOT EXISTS Sensorstatus (
        id INTEGER AUTO_INCREMENT PRIMARY KEY,
        sensor_id INTEGER,
        status VARCHAR(10),
        timestamp TIMESTAMP,
        FOREIGN KEY (sensor_id) REFERENCES Sensors(id)
    )
""")
print("Sensorstatus 테이블이 생성되었습니다.")

# Sensors 테이블에 데이터 추가
sensor_values = [
    ('Sensor1', 'carrot_area'),
    ('Sensor2', 'lectuce_area'),
    ('Sensor3', 'eggplant_area')
]

insert_query = "INSERT INTO Sensors (name, location) VALUES (%s, %s)"

cursor.executemany(insert_query, sensor_values)
print("데이터가 Sensors 테이블에 추가되었습니다.")

# 커밋 및 연결 종료
conn.commit()
conn.close()
