# DV2551-Project
![Class Diagram](https://www.planttext.com/plantuml/img/NP0n3eCm34NtdC8ZRSa5Eg5sgBfLS89G34GA2KgSgUhTQrM0CDdyl_F-Jwf49bAUd58r-WuZHZXh3IyxexCU6pA4qgf3eCOEtb2E1-iUFFCytNBVOsJaVXLqjCD8H_PpUcGBsc7FrafJwkrCIZ8BFWhuNAHQjFymk48RWYi1FKMBYUTiFPNGuKpZCftKToNBgXjQoiNkWcYzxGkrY4inZXCT_vYAtyOV_GC0)

```
@startuml

Renderer *-- PipelineState
Renderer *-- RootSignature
Renderer *-- ConstantBuffer

RootSignature -right- PipelineState
ImGui -left- Renderer

class RootSignature {
    + RootSignature(buffer : bool, entries : uint, depth : uint)
    + get() : ID3D12RootSignature *
}

class ConstantBuffer {
    + ConstantBuffer()
}

class PipelineState {
    + PipelineState(rootSign : RootSignature)
}

@enduml
```
