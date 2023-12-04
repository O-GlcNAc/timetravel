import pymysql.cursors

# Connect to the database
connection = pymysql.connect(host='localhost',
                             user='scott',
                             password='tiger',
                             database='exam',
                             cursorclass=pymysql.cursors.DictCursor)


def parse_sensor_data(data):
    parts = data.strip().split()
    sensor_id = int(parts[0])
    reading = float(parts[1])
    temperature = float(parts[2])
    humidity = float(parts[3])
    illuminance = float(parts[4])
    timestamp = parts[5] + " " + parts[6]  # timestamp 추가
    return sensor_id, reading, temperature, humidity, illuminance, timestamp

# Modify this part to read from FIFO and process the received data
with open('fifo', 'r') as fifo_file:
    while True:
        line = fifo_file.readline().strip()  # Read a line from FIFO
        if not line:
            continue

        sensor_id, reading, temperature, humidity, illuminance, timestamp = parse_sensor_data(line)  # Parse the received data

        with connection.cursor() as cursor:
            # Insert the parsed data into SensorData table
            # CURRENT_TIMESTAMP 대신 클라이언트에서 보낸 timestamp 사용
            sql = "INSERT INTO SensorData (sensor_id, reading, temperature, humidity, illuminance, timestamp) VALUES (%s, %s, %s, %s, %s, %s)"
            cursor.execute(sql, (sensor_id, reading, temperature, humidity, illuminance, timestamp))

        # Commit the changes to the database
        connection.commit()

        # Display the inserted data
        with connection.cursor() as cursor:
            sql = "SELECT * FROM SensorData WHERE sensor_id = %s AND reading = %s AND timestamp = %s"
            cursor.execute(sql, (sensor_id, reading, timestamp))
            result = cursor.fetchone()
            print(result)
