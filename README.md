# myIEC60870-5-104
利用esp32做的IEC60870-5-104，可以用wifi/有线以太网连接

# 说明
该软件在 arduino 上通过 IEC60870-5-104 实现通信，您可以使用"MASTER"和"SLAVE"对象的多个实例将 Arduino 与使用此协议的 PLC 或 RTU 连接起来。


# 已经实现的功能

## v1.0.1 支持的功能

| 功能码 | 描述 |
|--------|------|
| M_SP_NA_1 | 单点信息 |
| M_DP_NA_1 | 双点信息 |
| M_ME_NA_1 | 测量值，标准化值 |
| C_SC_NA_1 | 单命令 |
| C_DC_NA_1 | 双命令 |
| C_IC_NA_1 | 询问命令 |
| M_EI_NA_1 | 初始化结束 |

## 参数设置

通过命令`setParam(byte,bool)`，您可以设置要在连接建立时发送的电报：

* **0** - M_EI_NA_1 - 初始化结束
* **1** - C_IC_NA_1 - 询问命令（仅对 MASTER 实例有效）

## 硬件兼容性

该库已在 ESP32 上使用 WiFi 和以太网进行了测试。

### 启用有线模式

要启用 WIRED 模式，您需要：

1. 在 IEC60870-5-104.h 文件中添加 `#define IECWIRED`
2. 对于 ESP32，以太网库需要修改 Server.h 文件：
   * 将 `virtual void begin(uint16_t port=0) =0;` 
   * 更改为 `virtual void begin() =0;`（不带逗号）