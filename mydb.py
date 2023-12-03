import pymysql.cursors

# Connect to the database
connection = pymysql.connect(host='localhost',
                             user='scott',
                             password='tiger',
                             database='exam',
                             cursorclass=pymysql.cursors.DictCursor)

# Assume the received data is in the format: "sensor_id reading\n"
def parse_sensor_data(data):
    parts = data.strip().split()
    sensor_id = int(parts[0])
    reading = float(parts[1])
    temperature = float(parts[2])
    humidity = float(parts[3])
    illuminance = float(parts[4])
    return sensor_id, reading, temperature, humidity, illuminance

# Modify this part to read from FIFO and process the received data
with open('fifo', 'r') as fifo_file:
    while True:
        line = fifo_file.readline().strip()  # Read a line from FIFO
        if not line:
            continue

        sensor_id,reading,temperature,humidity,illuminance = parse_sensor_data(line)  # Parse the received data
        timestamp = None  # Since the timestamp is automatically added, no need to specify here

        with connection.cursor() as cursor:
            # Insert the parsed data into SensorData table
            sql = "INSERT INTO SensorData (sensor_id, reading,temperature,humidity,illuminance,timestamp) VALUES (%s, %s,%s,%s,%s, CURRENT_TIMESTAMP)"
            cursor.execute(sql, (sensor_id, reading, temperature,humidity,illuminance))

        # Commit the changes to the database
        connection.commit()

        # Display the inserted data
        with connection.cursor() as cursor:
            sql = "SELECT * FROM SensorData WHERE sensor_id = %s AND reading = %s"
            cursor.execute(sql, (sensor_id, reading))
            result = cursor.fetchone()
            print(result)
