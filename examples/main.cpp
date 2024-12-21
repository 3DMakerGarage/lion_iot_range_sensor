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

#include <Arduino.h>
#include <Lion.h>
#include <LionRangeSensor.h>

#define TEXT_SIZE 2

Lion lion = Lion();
LionRangeSensor sensor = LionRangeSensor();

LionRangeSensor::SensorData  lastestSersorData;
Deferred displayDrawDeferred = Deferred(100); // 0.1 seconds refresh time

void onSensorData(LionRangeSensor::SensorData data) {
    lastestSersorData = data;
}

void printSensorData() {
    lion.display_clear();
    lion.display_writeAlignedText(CENTER, MIDDLE, TEXT_SIZE,  (String)lastestSersorData.distance + "mm");
    lion.display_draw();
}

void onDisplayDeferredExecution() {
    printSensorData();
}

void setup() {
    lion.begin();
    sensor.setOnSensorDataEventCallback(onSensorData);
    sensor.begin();
}

void loop() {
    lion.loop();
    sensor.loop();
    displayDrawDeferred.execute(onDisplayDeferredExecution);
}