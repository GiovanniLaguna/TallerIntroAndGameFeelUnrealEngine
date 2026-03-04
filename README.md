# Aura

Proyecto de juego desarrollado en Unreal Engine 5.6 con sistema de habilidades basado en Gameplay Ability System (GAS).

## Descripción

Aura es un proyecto de juego de acción RPG que implementa un sistema completo de combate y habilidades utilizando el Gameplay Ability System de Unreal Engine. El proyecto incluye personajes jugables, enemigos, efectos visuales y un sistema de UI para mostrar atributos del jugador.

## Información del Proyecto

- **Motor**: Unreal Engine 5.6
- **Lenguaje**: C++ y Blueprints
- **Desarrollador**: Quetz-Lab
- **Copyright**: Quetz-Lab 2026

## Requisitos del Sistema

- Unreal Engine 5.6
- Visual Studio 2022 (recomendado)
- Windows 10/11
- DirectX 12

## Características Principales

### Sistema de Habilidades (Gameplay Ability System)
- Implementación completa del GAS de Unreal Engine
- Sistema de atributos replicados (Health, MaxHealth, Mana, MaxMana)
- Componente de sistema de habilidades personalizado
- Soporte para efectos de gameplay

### Arquitectura de Personajes
- Clase base abstracta `AAuraCharacterBase` que implementa `IAbilitySystemInterface`
- Personaje jugador (`AAuroPlayer`)
- Sistema de enemigos (`AAuraEnemy`) con interfaz de interacción
- Sistema de muerte y respawn

### Sistema de Atributos
- **Atributos Vitales**: Health, MaxHealth
- **Atributos Mágicos**: Mana, MaxMana
- Replicación de red para multijugador
- Notificaciones de cambio de atributos

### Sistema de UI
- Widgets personalizados con `AuraUserWidget`
- Controladores de widgets para separar lógica de presentación
- HUD del jugador con barras de vida y maná

### Sistema de Efectos
- Actores de efectos (`AuraEffectActor`) para aplicar modificadores
- Efectos visuales de combate (impactos, sangre, invocaciones)
- Sistema de cursor personalizado
- Efectos de fuego y tornado

### Assets Incluidos
- Personaje principal "Aura" con animaciones y materiales
- Conjunto de assets de mazmorras (bloques, esquinas, escaleras)
- Sistema de checkpoint y beacons
- Efectos de partículas (Niagara)

## Estructura del Proyecto

```
Aura/
├── Source/
│   └── Aura/
│       ├── Public/
│       │   ├── AbilitySystem/      # Sistema de habilidades GAS
│       │   ├── Actor/              # Actores del juego
│       │   ├── Game/               # GameMode y reglas
│       │   ├── Interaction/        # Interfaces de interacción
│       │   ├── Player/             # Clases del jugador
│       │   └── UI/                 # Sistema de interfaz
│       └── Private/                # Implementaciones
├── Content/
│   ├── Assets/
│   │   ├── Characters/            # Modelos y animaciones
│   │   ├── Dungeon/               # Assets de escenarios
│   │   └── Effects/               # Efectos visuales
│   ├── Blueprints/                # Blueprints del juego
│   └── Maps/                      # Mapas del juego
└── Config/                        # Configuración del proyecto
```

## Instalación

1. Clona el repositorio:
```bash
git clone [URL_DEL_REPOSITORIO]
```

2. Haz clic derecho en `Aura.uproject` y selecciona "Generate Visual Studio project files"

3. Abre `Aura.sln` en Visual Studio

4. Compila el proyecto en configuración "Development Editor"

5. Ejecuta el proyecto desde Visual Studio o abre `Aura.uproject` directamente

## Compilación

### Desde Visual Studio
```
Build > Build Solution (Ctrl+Shift+B)
```

### Desde línea de comandos
```bash
"C:\Program Files\Epic Games\UE_5.6\Engine\Build\BatchFiles\Build.bat" AuraEditor Win64 Development "RUTA_AL_PROYECTO\Aura.uproject" -waitmutex
```

## Configuración del Proyecto

### Plugins Habilitados
- **GameplayAbilities**: Sistema de habilidades de gameplay
- **ModelingToolsEditorMode**: Herramientas de modelado en el editor

### Dependencias del Módulo
- Core, CoreUObject, Engine
- InputCore, EnhancedInput
- GameplayAbilities
- GameplayTags, GameplayTasks
- UMG (para UI)

## Uso

### Mapa de Inicio
El mapa por defecto es `/Game/Maps/StartupMap`

### Controles
Los controles se configuran mediante Enhanced Input System. Revisa `Config/DefaultInput.ini` para personalizarlos.

## Desarrollo

### Añadir Nuevas Habilidades
1. Crea una nueva clase derivada de `UGameplayAbility`
2. Implementa la lógica en `ActivateAbility()`
3. Registra la habilidad en el `AuraAbilitySystemComponent`

### Añadir Nuevos Atributos
1. Añade el atributo en `AuraAttributeSet.h` usando la macro `ATTRIBUTE_ACCESSORS`
2. Implementa la función `OnRep_` correspondiente
3. Añade la replicación en `GetLifetimeReplicatedProps()`

### Crear Nuevos Efectos
1. Deriva de `AAuraEffectActor`
2. Configura los efectos de gameplay a aplicar
3. Implementa la lógica de activación/desactivación

## Arquitectura del Código

### Patrón de Diseño
- **MVC**: Separación entre Widgets (Vista) y WidgetControllers (Controlador)
- **Interface Segregation**: Uso de interfaces como `IEnemyInterface` para interacciones
- **Component-Based**: Uso extensivo del patrón de componentes de Unreal

### Replicación
El proyecto está preparado para multijugador con:
- Atributos replicados
- Componentes de habilidades replicados
- PlayerState para datos persistentes del jugador

## Contribuir

1. Crea un fork del proyecto
2. Crea una rama para tu feature (`git checkout -b feature/NuevaCaracteristica`)
3. Commit tus cambios (`git commit -m 'Añade nueva característica'`)
4. Push a la rama (`git push origin feature/NuevaCaracteristica`)
5. Abre un Pull Request

## Licencia

Copyright © Quetz-Lab 2026. Todos los derechos reservados.

## Contacto

**Quetz-Lab**
- Empresa: Quetz

## Notas Técnicas

### Configuración de Renderizado
- DirectX 12 como RHI por defecto
- Lumen habilitado para iluminación global dinámica
- Nanite habilitado para geometría virtualizada
- Virtual Shadow Maps habilitados
- Ray Tracing disponible (deshabilitado por defecto)

### Optimización
- Mesh Distance Fields generados para mejor oclusión
- Texture streaming habilitado
- Virtual Textures habilitados
- GPU Scene habilitado para mejor rendimiento

## Roadmap

- [ ] Sistema de inventario
- [ ] Más habilidades y efectos
- [ ] Sistema de progresión de personaje
- [ ] Multijugador completo
- [ ] Sistema de misiones
- [ ] IA mejorada para enemigos

## Problemas Conocidos

Consulta la sección de Issues en el repositorio para ver problemas conocidos y su estado.

##Controles Básicos

- Movimiento con WASD
- Click izquierdo para atacar
- Shift Izquierdo para Dash
- Q para Ataque en Área
- P para pausa

## Recursos Adicionales

- [Documentación de Unreal Engine 5.6](https://docs.unrealengine.com/)
- [Gameplay Ability System Documentation](https://docs.unrealengine.com/5.6/en-US/gameplay-ability-system-for-unreal-engine/)
- [Enhanced Input Documentation](https://docs.unrealengine.com/5.6/en-US/enhanced-input-in-unreal-engine/)
