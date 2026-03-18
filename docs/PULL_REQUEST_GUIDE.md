# Pull Request（PR）工作流指南

**[中文](#中文) | [English](#english)**

---

# 中文

## 什么是 Pull Request（PR）？

**Pull Request（PR，拉取请求）** 是 GitHub 上用于代码协作与合并的标准工作流。  
简单来说，PR 就是一个"代码变更提案"——开发者（或 Copilot 等 AI 助手）在一个独立分支上完成修改后，通过 PR 通知项目所有者："我有一批改动，请审核后决定是否合入主分支。"

```
feature/new-sync ──○──○──○──○──○
                                ↘  Pull Request
main            ──○──────────────●  (merge)
```

PR 并不是"直接改主分支"，而是先在独立分支上开发、测试、让团队审查，确认没问题后再合并到 `main`，从而保护主分支的稳定性。

---

## 为什么 Copilot 要创建 PR？

Copilot 在 `chengyicode/AbilityHelperSample` 里实现新功能时，会统一通过 PR 提交，原因如下：

| 原因 | 说明 |
|------|------|
| **安全隔离** | 新功能代码在独立分支开发，不影响主分支的可用性 |
| **可审查性** | 所有变更都有完整 diff，项目维护者可逐行审查 |
| **可追溯性** | PR 会记录每一次提交、讨论和决定，便于事后回查 |
| **可回滚性** | 如果发现问题，只需关闭/拒绝 PR，主分支完全不受影响 |
| **自动化记录** | Copilot 会在 PR 描述中写明改动内容、实现思路和注意事项 |

---

## PR 如何帮助安全集成新功能（以 DataTable-to-asset 自动同步为例）

本次 Copilot PR 的目标是在 `AbilityEditorHelper` 插件上新增通用 DataTable → 自定义资产同步能力（对齐已有的 GE/GA 处理逻辑）。整个过程如下：

```
步骤 1：Copilot 在独立分支 copilot/feature-datatable-asset-sync 开发
         ↓
步骤 2：Copilot 提交代码并开启 PR，填写说明文档
         ↓
步骤 3：维护者审查 PR（代码、测试、文档）
         ↓
步骤 4：可选：在 PR 上进行讨论、要求修改
         ↓
步骤 5：维护者确认没问题 → 点击 "Merge pull request" → 合并到 main
```

整个流程中，`main` 分支始终保持可用，只有在明确确认之后，新代码才会正式进入主线。

---

## PR 与直接修改主分支的对比

| 对比维度 | 直接修改 main | 通过 PR 合并 |
|----------|--------------|-------------|
| **安全性** | 风险高，错误直接影响主分支 | 安全，分支隔离 |
| **可审查性** | 无审查，无法在合入前拦截 | 完整 diff，逐行可审查 |
| **可回滚性** | 需要 `git revert`，操作复杂 | 未合并可直接关闭；已合并可 Revert PR |
| **记录与追溯** | 仅有提交记录，缺少上下文 | PR 含讨论、说明、审查意见 |
| **团队协作** | 容易产生冲突，难以协同 | 标准流程，团队可分工 |

**结论：对于任何涉及新功能、重构或批量变更的操作，都强烈推荐使用 PR。**

---

## Copilot PR 的自动化能力

Copilot 创建的 PR 具备以下自动化特性：

- **自动提交**：Copilot 会将所有相关修改整理为一系列 git 提交，并推送到专属分支。
- **变更记录**：PR 描述包含变更清单（Checklist），实时更新每一步完成情况。
- **可审查**：GitHub PR 页面展示完整 diff，支持按文件、按行查看每一处改动。
- **可讨论**：维护者和团队成员可以在 PR 的任意代码行上留下评论，Copilot 会根据反馈继续改进。
- **CI 集成**：如果项目配置了 CI（持续集成），PR 会自动触发构建和测试，确保合入前代码质量达标。

---

## PR 完成后如何合并到主分支

当你确认 PR 内容没有问题，合并步骤如下：

1. 打开 PR 页面（GitHub → Pull requests → 找到对应 PR）
2. 确认所有 CI 检查已通过（绿色 ✅）
3. 阅读并确认 PR 描述和 diff
4. 点击 **"Merge pull request"** 按钮
5. 选择合并方式：
   - **Create a merge commit**：保留完整分支历史（推荐）
   - **Squash and merge**：将所有提交压缩为一个（适合清理提交历史）
   - **Rebase and merge**：线性历史（适合追求简洁提交记录）
6. 点击 **"Confirm merge"**
7. 可选：删除已合并的分支（点击 "Delete branch"）

```
合并前：
main  ──●─────────────●
         \              
feature   ──○──○──○──○
                        ↑ PR

合并后：
main  ──●─────────────●──●  ← 新的合并提交
```

---

## 项目维护者如何管理 PR

### 审查 PR

- 在 GitHub PR 页面点击 **"Files changed"** 查看所有改动
- 对任意代码行点击 **"+"** 可以留下行级评论
- 在 **"Review changes"** 中选择：
  - **Comment**：留下评论（不阻止合并）
  - **Approve**：批准合并
  - **Request changes**：要求修改（阻止合并）

### 要求 Copilot 修改

在 PR 评论区描述需要的修改，Copilot 会读取反馈并继续更新代码。

### 拒绝 PR

如果改动不符合预期，维护者可以：

1. 点击 **"Close pull request"**（关闭但不删除分支，保留历史）
2. 可选：点击 "Delete branch" 删除对应分支

### 回滚已合并的 PR

如果合并后发现问题：

1. 打开已合并的 PR 页面
2. 点击 **"Revert"** 按钮
3. GitHub 会自动创建一个新的 PR，内容为撤销该 PR 的改动
4. 审查并合并这个 Revert PR，即可回滚

---

## 本次 Copilot PR 的预期效果

本次 PR（DataTable-to-asset 通用同步）完成后，`AbilityEditorHelper` 插件将具备：

| 能力 | 说明 |
|------|------|
| **通用 DataAsset 同步** | 支持自定义 `FMyConfig : FTableRowBase` → `UMyAsset : UPrimaryDataAsset` 的创建与更新 |
| **增量更新** | 仅处理自上次同步以来有变化的行，减少不必要的资产重建 |
| **目录清理** | 可选地删除 DataTable 中已不存在的资产 |
| **变更检测** | 使用序列化状态对比，仅在字段实际变化时标记为脏资产 |
| **后处理委托** | 提供 `OnPostProcessCustomAsset` 委托，支持自定义字段注入逻辑 |
| **与 GE/GA 对齐** | 新功能与现有 GE/GA 处理风格一致，学习成本低 |

### 推荐的使用流程

```
① 程序员定义配置结构体 FMyConfig : FTableRowBase
   + 定义目标资产类 UMyAsset : UPrimaryDataAsset

② 在 Editor Settings 中配置 MyAsset 的 DataTable 路径和输出目录

③ 策划在 Excel 中填写配置 → 导表工具生成 JSON → 导入 DataTable

④ 在编辑器中调用 "Create/Update MyAssets from DataTable"

⑤ 插件自动创建/更新 MyAsset 资产，仅变更行触发重建

⑥ 可选：启用 "Clear folder first" 清理已删除的配置行对应资产
```

---

## 参考资料

- [GitHub 官方文档：关于 Pull Request](https://docs.github.com/zh/pull-requests/collaborating-with-pull-requests/proposing-changes-to-your-work-with-pull-requests/about-pull-requests)
- [GitHub 官方文档：合并 Pull Request](https://docs.github.com/zh/pull-requests/collaborating-with-pull-requests/incorporating-changes-from-a-pull-request/merging-a-pull-request)
- [AbilityEditorHelper 插件使用指南](../Plugins/AbilityEditorHelper/AbilityEditorHelper插件使用指南.md)

---

---

# English

## What is a Pull Request (PR)?

A **Pull Request (PR)** is GitHub's standard workflow for code collaboration and merging.  
In simple terms, a PR is a "code change proposal" — after a developer (or an AI assistant like Copilot) finishes work on an isolated branch, a PR notifies the project owner: "I have a set of changes, please review them and decide whether to merge them into the main branch."

```
feature/new-sync ──○──○──○──○──○
                                ↘  Pull Request
main            ──○──────────────●  (merge)
```

A PR does **not** directly modify the main branch. Instead, development and testing happen on a separate branch. After the team reviews and approves it, the changes are merged into `main`, protecting the stability of the main branch.

---

## Why Does Copilot Create PRs?

When Copilot implements new features in `chengyicode/AbilityHelperSample`, it always submits changes via PRs for the following reasons:

| Reason | Description |
|--------|-------------|
| **Safe isolation** | New feature code is developed on an isolated branch, keeping the main branch untouched |
| **Reviewability** | All changes have a complete diff that maintainers can review line by line |
| **Traceability** | PRs record every commit, discussion, and decision for future reference |
| **Rollback safety** | If issues are found, simply close/reject the PR — the main branch is completely unaffected |
| **Automated documentation** | Copilot includes change descriptions, implementation notes, and a progress checklist in each PR |

---

## How PRs Help Safely Integrate New Features (DataTable-to-asset Sync Example)

The goal of this Copilot PR is to add a generic DataTable → custom asset sync capability to the `AbilityEditorHelper` plugin (aligned with the existing GE/GA handling logic). The process is:

```
Step 1: Copilot develops on an isolated branch: copilot/feature-datatable-asset-sync
         ↓
Step 2: Copilot commits code and opens a PR with documentation
         ↓
Step 3: Maintainer reviews the PR (code, tests, documentation)
         ↓
Step 4: Optional: discussion on the PR, requesting changes
         ↓
Step 5: Maintainer confirms everything is good → clicks "Merge pull request" → merged into main
```

Throughout this process, the `main` branch remains usable at all times. New code only officially enters the mainline after explicit confirmation.

---

## PR vs. Directly Modifying the Main Branch

| Dimension | Direct commit to main | Merge via PR |
|-----------|----------------------|--------------|
| **Safety** | High risk — errors directly affect the main branch | Safe — branch isolation |
| **Reviewability** | No review; cannot intercept issues before merging | Full diff — reviewable line by line |
| **Rollback** | Requires `git revert`; complex operations | Unmerged: just close the PR; Merged: use "Revert PR" |
| **Records & Traceability** | Only commit history, lacking context | PR includes discussion, descriptions, and review comments |
| **Team collaboration** | Prone to conflicts, hard to coordinate | Standard process, easy to divide work |

**Conclusion: For any new features, refactoring, or batch changes, using a PR is strongly recommended.**

---

## Copilot PR's Automated Capabilities

PRs created by Copilot have the following automated features:

- **Auto-commit**: Copilot organizes all related changes into a series of git commits and pushes them to a dedicated branch.
- **Change log**: The PR description contains a progress checklist that updates in real time as each step is completed.
- **Reviewable**: The GitHub PR page shows the full diff, allowing viewing by file or line for every change.
- **Discussable**: Maintainers and team members can leave comments on any line of code in the PR; Copilot will continue improving based on feedback.
- **CI integration**: If the project has CI (Continuous Integration) configured, the PR automatically triggers builds and tests to ensure code quality before merging.

---

## How to Merge a PR into the Main Branch

Once you've confirmed the PR content is ready, follow these steps to merge:

1. Open the PR page (GitHub → Pull requests → find the relevant PR)
2. Confirm all CI checks have passed (green ✅)
3. Read and confirm the PR description and diff
4. Click the **"Merge pull request"** button
5. Select a merge strategy:
   - **Create a merge commit**: Preserves complete branch history (recommended)
   - **Squash and merge**: Combines all commits into one (useful for cleaning up commit history)
   - **Rebase and merge**: Linear history (useful for clean commit records)
6. Click **"Confirm merge"**
7. Optional: Delete the merged branch (click "Delete branch")

```
Before merge:
main  ──●─────────────●
         \              
feature   ──○──○──○──○
                        ↑ PR

After merge:
main  ──●─────────────●──●  ← new merge commit
```

---

## How Project Maintainers Manage PRs

### Reviewing a PR

- On the GitHub PR page, click **"Files changed"** to see all changes
- Click **"+"** on any line of code to leave a line-level comment
- In **"Review changes"**, select:
  - **Comment**: Leave a comment (does not block merging)
  - **Approve**: Approve the merge
  - **Request changes**: Request modifications (blocks merging)

### Requesting Changes from Copilot

Describe the required changes in the PR comment section. Copilot will read the feedback and continue updating the code.

### Rejecting a PR

If the changes don't meet expectations, maintainers can:

1. Click **"Close pull request"** (closes but doesn't delete the branch, preserving history)
2. Optional: Click "Delete branch" to remove the associated branch

### Rolling Back a Merged PR

If issues are discovered after merging:

1. Open the already-merged PR page
2. Click the **"Revert"** button
3. GitHub automatically creates a new PR that undoes the changes from the original PR
4. Review and merge this Revert PR to complete the rollback

---

## Expected Results of This Copilot PR

After this PR (DataTable-to-asset generic sync) is complete, the `AbilityEditorHelper` plugin will have:

| Capability | Description |
|------------|-------------|
| **Generic DataAsset sync** | Supports creating and updating `UMyAsset : UPrimaryDataAsset` from `FMyConfig : FTableRowBase` |
| **Incremental updates** | Only processes rows that have changed since the last sync, reducing unnecessary asset rebuilds |
| **Directory cleanup** | Optionally deletes assets no longer present in the DataTable |
| **Change detection** | Uses serialization state comparison; only marks assets as dirty when fields actually change |
| **Post-processing delegate** | Provides an `OnPostProcessCustomAsset` delegate for custom field injection logic |
| **Aligned with GE/GA** | New feature matches the existing GE/GA handling style for minimal learning curve |

### Recommended Usage Workflow

```
① Programmer defines the config struct: FMyConfig : FTableRowBase
   + Defines the target asset class: UMyAsset : UPrimaryDataAsset

② Configure the MyAsset DataTable path and output directory in Editor Settings

③ Designer fills in configuration in Excel → export tool generates JSON → import into DataTable

④ In the editor, call "Create/Update MyAssets from DataTable"

⑤ Plugin automatically creates/updates MyAsset assets; only changed rows trigger rebuild

⑥ Optional: Enable "Clear folder first" to clean up assets for deleted config rows
```

---

## References

- [GitHub Docs: About Pull Requests](https://docs.github.com/en/pull-requests/collaborating-with-pull-requests/proposing-changes-to-your-work-with-pull-requests/about-pull-requests)
- [GitHub Docs: Merging a Pull Request](https://docs.github.com/en/pull-requests/collaborating-with-pull-requests/incorporating-changes-from-a-pull-request/merging-a-pull-request)
- [AbilityEditorHelper Plugin Guide](../Plugins/AbilityEditorHelper/AbilityEditorHelper插件使用指南.md)
