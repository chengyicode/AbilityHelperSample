# 路径配置说明（ExcelPath / JsonPath / SchemaPath）

本文档详细说明插件 **AbilityEditorHelper** 中三个核心路径配置的定义位置、使用位置、在数据流中的作用以及实际配置方式。

> **关于 `dataPath`**：代码中并不存在名为 `dataPath` 的独立字段。`ExcelPath` 和 `JsonPath` 在示例配置中指向同一个目录（`DataSample/`），该目录在概念上充当"数据根目录"。如果你在文档或对话中看到"dataPath"，它通常是对这两个路径所共同指向目录的非正式称呼。

---

## 1. 路径字段定义位置

三个路径均定义在同一个设置类中：

**文件**：`Plugins/AbilityEditorHelper/Source/AbilityEditorHelper/Public/AbilityEditorHelperSettings.h`

```cpp
// 第 99–111 行，Category = "DataPath"

/** Excel的数据存储路径 */
UPROPERTY(Config, BlueprintReadOnly, EditAnywhere, Category = "DataPath")
FString ExcelPath;

/** Json的数据存储路径 */
UPROPERTY(Config, BlueprintReadOnly, EditAnywhere, Category = "DataPath")
FString JsonPath;

/** Schema 导出路径 */
UPROPERTY(Config, BlueprintReadOnly, EditAnywhere, Category = "DataPath")
FString SchemaPath;
```

这三个字段随项目持久化到：

```
Config/DefaultEditorPerProjectUserSettings.ini
```

示例（来自本仓库实际配置）：

```ini
[/Script/AbilityEditorHelper.AbilityEditorHelperSettings]
ExcelPath="D:\Project\AbilityHelperSample\Plugins\AbilityEditorHelper\DataSample\"
JsonPath="D:\Project\AbilityHelperSample\Plugins\AbilityEditorHelper\DataSample\"
SchemaPath="D:\Project\AbilityHelperSample\Plugins\AbilityEditorHelper\Content\Python\Schema\"
```

---

## 2. 各路径的作用与使用位置

### 2.1 ExcelPath — Excel 文件目录

**作用**：指定 Excel 文件（`.xlsx` / `.csv`）的**存放目录**。

- 生成 Excel 模板时，生成的文件写入此目录。
- 导出 Excel 数据时，从此目录读取源文件。

**调用位置**：

| 文件 | 函数 | 用途 |
|---|---|---|
| `Plugins/AbilityEditorHelper/Content/Python/Editor/ability_editor_helper_python_library.py` | `GenerateExcelTemplateFromSchema()` | 将 Schema 生成的 Excel 模板写入 `ExcelPath` |
| `Plugins/AbilityEditorHelper/Content/Python/Editor/ability_editor_helper_python_library.py` | `ExportExcelToJsonUsingSchema()` | 从 `ExcelPath` 读取 Excel 源文件 |

**代码片段**（`ability_editor_helper_python_library.py` 第 15–19 行）：

```python
ability_editor_helper_settings = unreal.AbilityEditorHelperLibrary.get_ability_editor_helper_settings()
excel_path = ability_editor_helper_settings.excel_path   # 读取 ExcelPath 设置

excel_file_path = ability_editor_utils.normalize(os.path.join(excel_path, excel_file_name))
```

---

### 2.2 JsonPath — JSON 文件目录

**作用**：指定 JSON 数据文件的**输出/读取目录**。

- Excel 导出阶段：将转换后的 JSON 文件**写入**此目录。
- DataTable 导入阶段：从此目录**读取** JSON 文件，导入到 UE DataTable。

**调用位置**：

| 文件 | 函数 | 用途 |
|---|---|---|
| `Plugins/AbilityEditorHelper/Content/Python/Editor/ability_editor_helper_python_library.py` | `ExportExcelToJsonUsingSchema()` | 将 JSON 输出到 `JsonPath` 目录 |
| `Plugins/AbilityEditorHelper/Source/AbilityEditorHelper/Private/AbilityEditorHelperLibrary.cpp` | `ImportAndUpdateGameplayEffectsFromJson()` | 从 `JsonPath` 读取 JSON 导入 GE DataTable |
| `Plugins/AbilityEditorHelper/Source/AbilityEditorHelper/Private/AbilityEditorHelperLibrary.cpp` | `ImportAndUpdateGameplayAbilitiesFromJson()` | 从 `JsonPath` 读取 JSON 导入 GA DataTable |
| `Plugins/AbilityEditorHelper/Source/AbilityEditorHelper/Private/AbilityEditorHelperLibrary.cpp` | `ImportAndUpdateCustomDataAssetsFromJson()` | 从 `JsonPath` 读取 JSON 导入自定义 DataAsset |

**代码片段**（`AbilityEditorHelperLibrary.cpp` 第 1304–1312 行）：

```cpp
// 从设置获取 JsonPath 并拼接完整路径
const UAbilityEditorHelperSettings* Settings = GetDefault<UAbilityEditorHelperSettings>();
if (!Settings || Settings->JsonPath.IsEmpty())
{
    OutError = TEXT("UAbilityEditorHelperSettings 的 JsonPath 未配置");
    return false;
}
const FString JsonFilePath = FPaths::Combine(Settings->JsonPath, JsonFileName);
```

---

### 2.3 SchemaPath — Schema 文件目录

**作用**：指定 Python 工具**查找** `.schema.json` 文件的目录。

Schema 文件（`.schema.json`）是 UE 结构体的 JSON 描述，由 C++ 函数 `GenerateAllSchemasFromSettings` 生成，生成位置固定为插件内部目录 `Plugins/AbilityEditorHelper/Content/Python/Schema/`。**`SchemaPath` 设置应与该目录保持一致**，供 Python 工具在生成 Excel 模板和导出 JSON 时定位对应的 Schema 文件。

**调用位置**：

| 文件 | 函数 | 用途 |
|---|---|---|
| `Plugins/AbilityEditorHelper/Content/Python/Editor/ability_editor_helper_python_library.py` | `GenerateExcelTemplateFromSchema()` | 在 `SchemaPath` 目录查找指定结构体的 Schema 文件 |
| `Plugins/AbilityEditorHelper/Content/Python/Editor/ability_editor_helper_python_library.py` | `ExportExcelToJsonUsingSchema()` | 在 `SchemaPath` 目录查找 Schema，用于解析 Excel |
| `Plugins/AbilityEditorHelper/Content/Python/ability_editor_utils.py` | `find_schema_file()` | 按结构体名在目录下依次查找 `<Name>.schema.json`、`<Name>.json`、`<Name>.schema` |

**代码片段**（`ability_editor_utils.py` 第 50–68 行）：

```python
def find_schema_file(schema_dir: str, struct_type_name: str):
    candidates = [
        os.path.join(schema_dir, f"{struct_type_name}.schema.json"),
        os.path.join(schema_dir, f"{struct_type_name}.json"),
        os.path.join(schema_dir, f"{struct_type_name}.schema"),
    ]
    for p in candidates:
        if os.path.isfile(p):
            return True, p
    return False, f"Schema 目录中找不到 {struct_type_name} 的 schema 文件"
```

---

## 3. 数据流中的角色关系

```
┌──────────────────────────────────────────────────────────────────────┐
│                         完整数据流水线                                │
│                                                                      │
│  SchemaPath                                                          │
│  └─ *.schema.json ──► GenerateExcelTemplateFromSchema()             │
│                              │                                       │
│                              ▼                                       │
│  ExcelPath                                                           │
│  └─ *.xlsx / *.csv   ◄── 生成模板（策划填写数据）                    │
│         │                                                            │
│         │  ExportExcelToJsonUsingSchema()                            │
│         │  （同时读取 SchemaPath 的 Schema 做格式解析）              │
│         ▼                                                            │
│  JsonPath                                                            │
│  └─ *.json ──► ImportAndUpdate*FromJson()                           │
│                       │                                              │
│                       ▼                                              │
│               UE DataTable / GE / GA 资产                           │
└──────────────────────────────────────────────────────────────────────┘
```

| 路径 | 输入/输出 | 阶段 |
|---|---|---|
| `SchemaPath` | 输入（读取） | 生成模板阶段 & 导出 JSON 阶段 |
| `ExcelPath` | 输出（写入模板）→ 输入（读取数据） | 生成模板阶段 & 导出 JSON 阶段 |
| `JsonPath` | 输出（写入 JSON）→ 输入（读取导入） | 导出 JSON 阶段 & 导入 DataTable 阶段 |

---

## 4. 配置方式

### 4.1 在编辑器中配置

打开 **Edit → Project Settings → Plugins → Ability Editor Helper**，在 **DataPath** 分类下填写三个路径，保存后自动写入 `Config/DefaultEditorPerProjectUserSettings.ini`。

![Schema 路径配置](../Resources/schema_path_config.png)

### 4.2 直接编辑 INI 文件

```ini
[/Script/AbilityEditorHelper.AbilityEditorHelperSettings]
; Excel 文件目录（绝对路径，末尾带斜杠）
ExcelPath="C:\MyProject\Data\Excel\"

; JSON 文件目录（绝对路径，末尾带斜杠）
JsonPath="C:\MyProject\Data\Json\"

; Schema 文件目录（通常指向插件内部 Schema 目录）
SchemaPath="C:\MyProject\Plugins\AbilityEditorHelper\Content\Python\Schema\"
```

### 4.3 路径要求

| 字段 | 类型 | 要求 |
|---|---|---|
| `ExcelPath` | 目录路径 | 目录需存在；末尾可带 `\` 或 `/` |
| `JsonPath` | 目录路径 | 目录不存在时 Python 工具会自动创建（`os.makedirs`） |
| `SchemaPath` | 目录路径 | 目录必须存在且包含对应 `.schema.json` 文件 |

> **建议**：`ExcelPath` 和 `JsonPath` 可以设置为同一目录（如示例配置中的 `DataSample/`），便于集中管理数据文件；`SchemaPath` 通常固定指向插件的 `Content/Python/Schema/` 目录。

### 4.4 查看 Schema 文件示例

Schema 文件位于：

```
Plugins/AbilityEditorHelper/Content/Python/Schema/
```

可以用 `UAbilityEditorHelperLibrary::GenerateAllSchemasFromSettings` 自动生成到该目录，然后把 `SchemaPath` 指向它。

---

## 5. 常见错误与排查

| 错误信息 | 原因 | 解决方案 |
|---|---|---|
| `UAbilityEditorHelperSettings 的 JsonPath 未配置` | `JsonPath` 字段为空 | 在 Project Settings 中填写 `JsonPath` |
| `Excel 文件不存在：<path>` | `ExcelPath` 目录或文件名错误 | 确认 `ExcelPath` 目录存在且文件名正确 |
| `Schema 路径不是目录或不存在` | `SchemaPath` 指向的目录不存在 | 先执行 `GenerateAllSchemasFromSettings` 生成 Schema |
| `Schema 目录中找不到 <Name> 的 schema 文件` | Schema 文件未生成或结构体名称拼写错误 | 执行一次 Schema 导出，确认结构体路径配置正确 |
