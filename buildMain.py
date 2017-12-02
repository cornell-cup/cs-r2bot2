import subprocess

subprocess.call(['g++','-pthread','-std=c++11','-Wall','-c','-I','R2Bot/include','-I','R2BotPiStandalone/include','-I','lib/boost_1_63_0','-I','lib/crow/amalgamate','-I','lib/cs-communication-utilities/Utilities','-I','lib/cs-r2-protocol/src','-I','lib/opencv3/opencv/build/include','-I','lib/sqlite','-I','lib/rplidar/sdk/sdk/include','R2BotPiStandalone/src/main.cpp','-o','obj/main.o'])

subprocess.call(['g++','-pthread','-std=c++11','-Wall','obj/SafetyHandler.o','obj/Job.o','obj/LidarSensor.o','obj/PathPlanningHandler.o','obj/HeadHandler.o','obj/SoundHandler.o','obj/Grid.o','obj/UltrasoundSensor.o','obj/IMUSensor.o','obj/DrawerSensor.o','obj/ForwardHandler.o','obj/FakeMotorController.o','obj/HeadFlapController.o','obj/R2Databases.o','obj/Obstacle.o','obj/Dijkstra.o','obj/HeadFlapHandler.o','obj/Sensor.o','obj/SoundController.o','obj/Path.o','obj/Coord.o','obj/JobHandler.o','obj/R2Server.o','obj/UDPServerSensor.o','obj/HeadSensor.o','obj/ManualInputsHandler.o','obj/MotorController.o','obj/PowerHandler.o','obj/Controller.o','obj/RFIDSensor.o','obj/Global.o','obj/UDPClientController.o','obj/main.o','-L','lib/rplidar/sdk/output/Linux/Release','-lboost_system','-lboost_date_time','-lsqlite3','-lrplidar_sdk','-o','bin/R2BotPiStandalone.x'])


