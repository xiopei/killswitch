Introduction

A simple yet effective VST plugin for the Killswitch technique.

Inspired by the lack of a built-in killswitch function in Ample Sound guitar libraries, I decided to develop this plugin (with a little help from AI) to fill that gap. The plugin is triggered by external MIDI input: any MIDI Note-On message activates the "Kill" state, and Note-Off releases it.

Physical Modeling & DSP Logic

This is not just a simple volume muting tool. To capture the raw, "electric" feel of a mechanical switch, this plugin models the following physical characteristics:

Transient DC Offset (Breaking the Circuit): In real guitar circuitry, there is often a slight voltage difference (charge accumulation) between the pickups, cables, and the amplifier input. When the signal is suddenly shorted to ground, these accumulated charges are released instantly. This plugin simulates this by injecting a DC bias pulse at the moment the button is pressed. At the moment the circuit closes (button released), the transient change in contact resistance is modeled by another DC bias pulse, recreating the aggressive "click" sound that makes the killswitch technique so distinctive in rock and metal music.

Environmental Noise Simulation: While the button is held down (Muted), the plugin adds a subtle layer of white noise to simulate the thermal noise and electromagnetic interference typical of a high-gain signal chain.

简介

一个简易但够用的kill switch技法实现效果器。

由于 Ample Sound 的吉他音源缺少内置kill switch功能，我借助AI开发这款插件来填补这一空白。该插件由外部 MIDI 输入触发：任何 MIDI 音符开启（Note-On）信号都会激活“静音”状态，而音符关闭（Note-Off）信号则会解除静音状态。

物理建模与 DSP 逻辑

这不是一个简单的静音工具。为了还原电路的真实听感，本插件模拟了以下物理特性：

1.瞬态直流偏移（电路断开）：在真实的吉他电路中，拾音器、连接线和放大器输入之间通常存在轻微的电压差（电荷积累）。当信号突然接地时，这些积累的电荷会瞬间释放。本插件通过在按下按钮的瞬间注入直流偏置脉冲来模拟这一现象。同时在电路闭合（按钮松开）的瞬间，触点电阻的瞬态变化会通过另一个直流偏置脉冲进行模拟，从而模拟摇滚和金属音乐中标志性的“Click”声音。

2.环境噪声模拟：按住按钮（静音）时，插件会添加轻微的白噪声，以模拟高增益信号链中常见的热噪声和电磁干扰。
