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

#ifndef LionRangeSensor_h
#define LionRangeSensor_h

#include <Arduino.h>
#include <Wire.h>
#include <Deferred.h>

#define RANGE_SENSOR_ADDRESS 0x52
#define RANGE_SENSOR_SAMPLING_TIME 100

typedef void (*waitForCallback)();

class LionRangeSensor : private OnDeferredExecutionCallback {
     public:
        struct SensorData {
            unsigned short distance = 0;
        };
        
        typedef void(*OnSensorDataEvent)(SensorData);

        void setOnSensorDataEventCallback(OnSensorDataEvent callback);
        void begin();
        void loop();

    private:
        Deferred deferredLoop = Deferred(RANGE_SENSOR_SAMPLING_TIME);
        OnSensorDataEvent onSensorDataEventCallback;        

        virtual void onDeferredExecution();
        unsigned short readDistance();
};

#endif