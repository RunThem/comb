<p align="center">
<!--
  <a href="https://github.com/RunThem/comb/">
    <img src="images/logo.png" alt="Logo" width="80" height="80">
  </a>
-->

  <h3 align="center">comb</h3>
  <p align="center">一个简单的解析器组合子库!</p>
</p>
<br />

根据[`comb`](https://github.com/wbhart/comb) 实现的一个简单的解析器组合子库.
 
## 目录

- [上手指南](#上手指南)
  - [开发前的配置要求](#配置要求)
  - [安装步骤](#安装步骤)
- [作者](#作者)
- [鸣谢](#鸣谢)

### 上手指南

将本仓库克隆至本地并改名.

###### 配置要求

1. [xmake](https://github.com/xmake-io/xmake) 项目构建工具
2. [c2x compiler](https://zh.cppreference.com/w/c/23) 支持 `c2x` 标准的C编译器

###### **安装步骤**

```shell
git clone https://github.com/RunThem/comb.git
cd comb

xmake f -m debug --toolchain=clang && xmake lsp && xmake -v
```

### 作者

<a href="mailto:iccy.fun@outlook.com">RunThem</a>

### 版权说明

该项目签署了 `MIT` 授权许可, 详情请参阅 [LICENSE](https://github.com/RunThem/mpc/blob/master/LICENSE)

### 鸣谢

- [xmake](https://github.com/xmake-io/xmake)
- [cc](https://github.com/JacksonAllan/CC)
- [sds](https://github.com/antirez/sds)
- [mimalloc](https://github.com/microsoft/mimalloc)
- [dbg-macro](https://github.com/eerimoq/dbg-macro)
- [tiny-regex-c](https://github.com/kokke/tiny-regex-c)
