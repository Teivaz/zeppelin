#pragma once

/**
 * Advertising Data Structures
 * https://www.bluetooth.com/specifications/assigned-numbers/generic-access-profile/
 */
enum AdvData {
	EAdvData_Flags =                      0x01, // Flags
	EAdvData_Services16 =                 0x02, // Incomplete List of 16-bit Service Class UUIDs
	EAdvData_Services16Complete =         0x03, // Complete List of 16-bit Service Class UUIDs
	EAdvData_Services32 =                 0x04, // Incomplete List of 32-bit Service Class UUIDs
	EAdvData_Services32Complete =         0x05, // Complete List of 32-bit Service Class UUIDs
	EAdvData_Services128 =                0x06, // Incomplete List of 128-bit Service Class UUIDs
	EAdvData_Services128Complete =        0x07, // Complete List of 128-bit Service Class UUIDs
	EAdvData_Name =                       0x08, // Shortened Local Name
	EAdvData_NameComplete =               0x09, // Complete Local Name
	EAdvData_TxPower =                    0x0A, // Tx Power Level
	EAdvData_Class =                      0x0D, // Class of Device
	EAdvData_PairingHashC =               0x0E, // Simple Pairing Hash C
	EAdvData_PairingHashC192 =            0x0E, // Simple Pairing Hash C-192
	EAdvData_PairingHashR =               0x0F, // Simple Pairing Randomizer R
	EAdvData_PairingHashR192 =            0x0F, // Simple Pairing Randomizer R-192
	EAdvData_DeviceId =                   0x10, // Device ID
	EAdvData_SecTkValue =                 0x10, // Security Manager TK Value
	EAdvData_SecOutOfBandFlags =          0x11, // Security Manager Out of Band Flags
	EAdvData_ConnectionIntervalRange =    0x12, // Slave Connection Interval Range
	EAdvData_Services16Solicitation =     0x14, // List of 16-bit Service Solicitation UUIDs
	EAdvData_Services128Solicitation =    0x15, // List of 128-bit Service Solicitation UUIDs
	EAdvData_ServiceData =                0x16, // Service Data
	EAdvData_PublicTargetAddr =           0x17, // Public Target Address
	EAdvData_RandomTargetAddr =           0x18, // Random Target Address
	EAdvData_Appearance =                 0x19, // Appearance
	EAdvData_AdvInterval =                0x1A, // Advertising Interval
	EAdvData_LeDeviceAddr =               0x1B, // LE Bluetooth Device Address	
	EAdvData_LeRole =                     0x1C, // LE Role
	EAdvData_PairingC256 =                0x1D, // Simple Pairing Hash C-256
	EAdvData_PairingR256 =                0x1E, // Simple Pairing Randomizer R-256
	EAdvData_Services32Solicitation =     0x1F, // List of 32-bit Service Solicitation UUIDs
	EAdvData_ServiceData32 =              0x20, // Service Data - 32-bit UUID
	EAdvData_ServiceData128 =             0x21, // Service Data - 128-bit UUID
	EAdvData_LeSecConConfirmationVal =    0x22, // LE Secure Connections Confirmation Value
	EAdvData_LeSecConRandomVal =          0x23, // LE Secure Connections Random Value
	EAdvData_Uri =                        0x24, // URI
	EAdvData_IndoorPositioning =          0x25, // Indoor Positioning
	EAdvData_TransportDiscoveryData =     0x26, // Transport Discovery Data
	EAdvData_LeSupportedFeatures =        0x27, // LE Supported Features
	EAdvData_ChannelMapUpdateIndication = 0x28, // Channel Map Update Indication
	EAdvData_PbAdv =                      0x29, // PB-ADV
	EAdvData_MeshMessage =                0x2A, // Mesh Message
	EAdvData_MeshBeacon =                 0x2B, // Mesh Beacon
	EAdvData_BigInfo =                    0x2C, // BIGInfo
	EAdvData_BroadcastCode =              0x2D, // Broadcast_Code
	EAdvData_InformationData3d =          0x3D, // 3D Information Data
	EAdvData_ManufacturerSpecificData =   0xFF, // Manufacturer Specific Data
};

/**
 * The GATT Appearance Characteristic
 * https://www.bluetooth.com/wp-content/uploads/Sitecore-Media-Library/Gatt/Xml/Characteristics/org.bluetooth.characteristic.gap.appearance.xml
 */
enum CharAppearance {
	ECharAppearance_Unknown =                            0, // None
	ECharAppearance_GenericPhone =                       64, // Generic category
	ECharAppearance_GenericComputer =                    128, // Generic category
	ECharAppearance_GenericWatch =                       192, // Generic category
	ECharAppearance_SportsWatch =                        193, // Watch subtype
	ECharAppearance_GenericClock =                       256, // Generic category
	ECharAppearance_GenericDisplay =                     320, // Generic category
	ECharAppearance_GenericRemoteControl =               384, // Generic category
	ECharAppearance_GenericEyeGlasses =                  448, // Generic category
	ECharAppearance_GenericTag =                         512, // Generic category
	ECharAppearance_GenericKeyring =                     576, // Generic category
	ECharAppearance_GenericMediaPlayer =                 640, // Generic category
	ECharAppearance_GenericBarcodeScanner =              704, // Generic category
	ECharAppearance_GenericThermometer =                 768, // Generic category
	ECharAppearance_ThermometerEar =                     769, // Thermometer subtype
	ECharAppearance_GenericHeartRateSensor =             832, // Generic category
	ECharAppearance_HeartRateBelt =                      833, // Heart Rate Sensor subtype
	ECharAppearance_GenericBloodPressure =               896, // Generic category
	ECharAppearance_BloodPressureArm =                   897, // Blood Pressure subtype
	ECharAppearance_BloodPressureWrist =                 898, // Blood Pressure subtype
	ECharAppearance_HumanInterfaceDevice =               960, // HID Generic
	ECharAppearance_Keyboard =                           961, // HID subtype
	ECharAppearance_Mouse =                              962, // HID subtype
	ECharAppearance_Joystick =                           963, // HID subtype
	ECharAppearance_Gamepad =                            964, // HID subtype
	ECharAppearance_DigitizerTablet =                    965, // HID subtype
	ECharAppearance_CardReader =                         966, // HID subtype
	ECharAppearance_DigitalPen =                         967, // HID subtype
	ECharAppearance_BarcodeScanner =                     968, // HID subtype
	ECharAppearance_GenericGlucoseMeter =                1024, // Generic category
	ECharAppearance_GenericRunningWalkingSensor =        1088, // Generic category
	ECharAppearance_RunningWalkingSensorInShoe =         1089, // Running Walking Sensor subtype
	ECharAppearance_RunningWalkingSensorOnShoe =         1090, // Running Walking Sensor subtype
	ECharAppearance_RunningWalkingSensorOnHip =          1091, // Running Walking Sensor subtype
	ECharAppearance_GenericCycling =                     1152, // Generic category
	ECharAppearance_CyclingComputer =                    1153, // Cycling subtype
	ECharAppearance_CyclingSpeedSensor =                 1154, // Cycling subtype
	ECharAppearance_CyclingCadenceSensor =               1155, // Cycling subtype
	ECharAppearance_CyclingPowerSensor =                 1156, // Cycling subtype
	ECharAppearance_CyclingSpeedAndCadenceSensor =       1157, // Cycling subtype
	ECharAppearance_GenericPulseOximeter =               3136, // Pulse Oximeter Generic Category
	ECharAppearance_Fingertip =                          3137, // Pulse Oximeter subtype
	ECharAppearance_WristWorn =                          3138, // Pulse Oximeter subtype
	ECharAppearance_GenericWeightScale =                 3200, // Weight Scale Generic Category
	ECharAppearance_GenericPersonalMobilityDevice =      3264, // Personal Mobility Device
	ECharAppearance_PoweredWheelchair =                  3265, // Personal Mobility Device
	ECharAppearance_MobilityScooter =                    3266, // Personal Mobility Device
	ECharAppearance_GenericContinuousGlucoseMonitor =    3328, // Continuous Glucose Monitor
	ECharAppearance_GenericInsulinPump =                 3392, // Insulin Pump
	ECharAppearance_InsulinPumpDurable =                 3393, // Insulin Pump
	ECharAppearance_InsulinPumpPatch =                   3396, // Insulin Pump
	ECharAppearance_InsulinPen =                         3400, // Insulin Pump
	ECharAppearance_GenericMedicationDelivery =          3456, // Medication Delivery
	ECharAppearance_GenericOutdoorSportsActivity =       5184, // Outdoor Sports Activity Generic Category
	ECharAppearance_LocationDisplayDevice =              5185, // Outdoor Sports Activity subtype
	ECharAppearance_LocationAndNavigationDisplayDevice = 5186, // Outdoor Sports Activity subtype
	ECharAppearance_LocationPod =                        5187, // Outdoor Sports Activity subtype
	ECharAppearance_LocationAndNavigationPod =           5188, // Outdoor Sports Activity subtype
};

/**
 * Predefined GATT Services UUID 16
 * https://www.bluetooth.com/specifications/gatt/services/
 */

enum GattService {
	EGattService_GenericAccess =                  0x1800, // org.bluetooth.service.generic_access
	EGattService_AlertNotificationService =       0x1811, // org.bluetooth.service.alert_notification
	EGattService_AutomationIo =                   0x1815, // org.bluetooth.service.automation_io
	EGattService_BatteryService =                 0x180F, // org.bluetooth.service.battery_service
	EGattService_BinarySensor =                   0x183B, // GATT Service UUID
	EGattService_BloodPressure =                  0x1810, // org.bluetooth.service.blood_pressure
	EGattService_BodyComposition =                0x181B, // org.bluetooth.service.body_composition
	EGattService_BondManagementService =          0x181E, // org.bluetooth.service.bond_management
	EGattService_ContinuousGlucoseMonitoring =    0x181F, // org.bluetooth.service.continuous_glucose_monitoring
	EGattService_CurrentTimeService =             0x1805, // org.bluetooth.service.current_time
	EGattService_CyclingPower =                   0x1818, // org.bluetooth.service.cycling_power
	EGattService_CyclingSpeedAndCadence =         0x1816, // org.bluetooth.service.cycling_speed_and_cadence
	EGattService_DeviceInformation =              0x180A, // org.bluetooth.service.device_information
	EGattService_EmergencyConfiguration =         0x183C, // GATT Service UUID
	EGattService_EnvironmentalSensing =           0x181A, // org.bluetooth.service.environmental_sensing
	EGattService_FitnessMachine =                 0x1826, // org.bluetooth.service.fitness_machine
	EGattService_GenericAttribute =               0x1801, // org.bluetooth.service.generic_attribute
	EGattService_Glucose =                        0x1808, // org.bluetooth.service.glucose
	EGattService_HealthThermometer =              0x1809, // org.bluetooth.service.health_thermometer
	EGattService_HeartRate =                      0x180D, // org.bluetooth.service.heart_rate
	EGattService_HttpProxy =                      0x1823, // org.bluetooth.service.http_proxy
	EGattService_HumanInterfaceDevice =           0x1812, // org.bluetooth.service.human_interface_device
	EGattService_ImmediateAlert =                 0x1802, // org.bluetooth.service.immediate_alert
	EGattService_IndoorPositioning =              0x1821, // org.bluetooth.service.indoor_positioning
	EGattService_InsulinDelivery =                0x183A, // org.bluetooth.service.insulin_delivery
	EGattService_InternetProtocolSupportService = 0x1820, // org.bluetooth.service.internet_protocol_support
	EGattService_LinkLoss =                       0x1803, // org.bluetooth.service.link_loss
	EGattService_LocationAndNavigation =          0x1819, // org.bluetooth.service.location_and_navigation
	EGattService_MeshProvisioningService =        0x1827, // org.bluetooth.service.mesh_provisioning
	EGattService_MeshProxyService =               0x1828, // org.bluetooth.service.mesh_proxy
	EGattService_NextDstChangeService =           0x1807, // org.bluetooth.service.next_dst_change
	EGattService_ObjectTransferService =          0x1825, // org.bluetooth.service.object_transfer
	EGattService_PhoneAlertStatusService =        0x180E, // org.bluetooth.service.phone_alert_status
	EGattService_PulseOximeterService =           0x1822, // org.bluetooth.service.pulse_oximeter
	EGattService_ReconnectionConfiguration =      0x1829, // org.bluetooth.service.reconnection_configuration
	EGattService_ReferenceTimeUpdateService =     0x1806, // org.bluetooth.service.reference_time_update
	EGattService_RunningSpeedAndCadence =         0x1814, // org.bluetooth.service.running_speed_and_cadence
	EGattService_ScanParameters =                 0x1813, // org.bluetooth.service.scan_parameters
	EGattService_TransportDiscovery =             0x1824, // org.bluetooth.service.transport_discovery
	EGattService_TxPower =                        0x1804, // org.bluetooth.service.tx_power
	EGattService_UserData =                       0x181C, // org.bluetooth.service.user_data
	EGattService_WeightScale =                    0x181D, // org.bluetooth.service.weight_scale
};

enum DataTypeRole {
	EDataTypeRole_OnlyPeripheral =       0x00,
	EDataTypeRole_OnlyCentral =          0x01,
	EDataTypeRole_PeripheralAndCentral = 0x02, // Peripheral preferred
	EDataTypeRole_CentralAndPeripheral = 0x03, // Central preferred
};

/**
 * This is not an eaxhaustive list
 * https://www.bluetooth.com/specifications/assigned-numbers/company-identifiers/
 */
enum BtCompanyId {
	EBtCompanyId_Valve = 0x055D, // Valve Corporation
	EBtCompanyId_Apple = 0x004C, // Apple, Inc.
} BtCompanyId;

