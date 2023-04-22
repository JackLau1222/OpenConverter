git config --global user.name "你的用户名"
git config --global user.email "你的邮箱"

git clone URL //拉取远程仓库
touch test.cpp //添加一个名为test的cpp文件
git add test.cpp //将此更改提交到缓冲区
git commit -a -m “add test.cpp” //将更改提交到本地仓库
git push //相当于pull request
