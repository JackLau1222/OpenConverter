# 贡献指南

欢迎您对本仓库做出贡献！非常感谢您的帮助，以下是一些建议和指导，以确保贡献的顺利进行。

## 贡献方式

您可以通过以下方式贡献：

1. **报告问题**：发现了文档中的错误或有改进的地方？请创建一个 Issue 来报告问题。

2. **提出改进建议**：如果您有关于文档结构、内容或样式的建议，请通过 Issue 提出您的想法。

3. **提交项目**：如果您有自己的项目并希望分享，欢迎提交 Pull Request。

## 报告问题和提出建议

1. 在我们的 [Issues 页面](https://github.com/JackLau1222/OpenConverter/issues) 创建一个新的 Issue。

2. 选择相应的标签，例如“bug”或“enhancement”。

3. 提供清晰而详细的说明，包括问题的复现步骤（如果是报告问题）或您的建议。

## 提交项目

1. Fork 这个仓库并在本地克隆它的develop分支：

    ```bash
   git clone -b develop https://github.com/your-username/your-repo-name.git
   ```
2. 在此目录下新建目录，以内容命名
    ```bash
    mkdir your_directory
    ```
3. 进入您刚刚创建的目录，将您的新项目放入到目录下并创建编辑您的markdown文档
    ```bash
    cd ./your_directory
    touch README.md
    vim README.md
    ```
4. 提交您的修改
    ```bash
    git add .
    git commit -m "Add/modify your-documentation"
    ```
5. 推送到您的Fork
    ```bash
    git push origin feature/your-feature
    ```
6. 创建一个 Pull Request（PR）：

- 到您的 Fork 页面，点击 "New Pull Request"。
- 选择要将您的修改合并到的目标分支。
- 提供清晰而简洁的 PR 描述。
7. 我们将审查您的 PR，并在需要时提出建议或请求修改。

## 注意事项
- 请确保您的文档使用正确的 Markdown 语法，并注意格式的一致性。
- 如果您贡献的是新的主题，请在目录中添加相应的链接。
- 我们鼓励包含适当的图片、图表等以提高文档的可读性。如有图片请将图片放入image目录下。
- 不要提交二进制可执行文件或者目标文件等
- 命名规范：不能有中文、空格、特殊字符

- > 项目上传需要：
  >  1.代码文件
  >  2.项目信息文件（sln等）
  >  3.说明文档（md）