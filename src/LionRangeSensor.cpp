/*
 * Copyright 3D Maker Garage 2024
 *
 * This file is part of some open source application.
 *
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 *
 * Contact: 3dmakergarage@gmail.com
 */

#include <LionRangeSensor.h>

void LionRangeSensor::begin(unsigned long samplingTime) {
    Wire.begin();
    deferredLoop = Deferred(samplingTime);
}

void LionRangeSensor::setOnSensorDataEventCallback(OnSensorDataEvent callback) {
    onSensorDataEventCallback = callback;
}

void LionRangeSensor::loop() {
    deferredLoop.execute(this);
}

void LionRangeSensor::onDeferredExecution() {
    if (onSensorDataEventCallback) {
        SensorData sensorData;
        sensorData.distance = readDistance();
        onSensorDataEventCallback(sensorData);
    }
}

unsigned short LionRangeSensor::readDistance() {
    unsigned short distance = 0;
    unsigned char i2c_rx_buf[16];
    unsigned char* datbuf = i2c_rx_buf;

    // step 1: instruct sensor to read echoes
    //      transmit to device #82 (0x52)
    Wire.beginTransmission(RANGE_SENSOR_ADDRESS); 
    // the address specified in the datasheet is 164 (0xa4)
    // but i2c adressing uses the high 7 bits so it's 82
    Wire.write(byte(0x00));         // sets distance data address (addr)
    Wire.endTransmission();         // stop transmitting
    // step 2: wait for readings to happen
    delay(1);                       // datasheet suggests at least 30uS
    // step 3: request reading from sensor
    //      request cnt bytes from slave device #82 (0x52)
    Wire.requestFrom(RANGE_SENSOR_ADDRESS, 2);  
    // step 5: receive reading from sensor
    if (2 <= Wire.available()) {    // if two bytes were received
        *datbuf++ = Wire.read();    // receive high byte (overwrites previous reading)
        *datbuf++ = Wire.read();    // receive low byte as lower 8 bits
    }

    distance=i2c_rx_buf[0];
    distance=distance<<8;
    distance|=i2c_rx_buf[1];
    return distance;
}
