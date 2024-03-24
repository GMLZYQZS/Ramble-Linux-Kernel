# 漫谈Linux系统内核(Ramble-Linux-Kernel)
>该项目旨在对Linux内核代码的研究与讨论
### 项目目标阶段划分：
1.第一阶段：以解读0.11内核为主，进行内核释义与原因解读。实现步骤详见“第一阶段实现步骤”。</br>
2.第二阶段：以补充0.11内核为主，并将补充的算法与0.12内核对比，进行个性化。实现步骤详见“第二阶段实现步骤”。</br>
3.第三阶段：对3.4.2进行释义且补充算法，在此阶段可尝试其他实现方式。</br>
4.第四阶段：对当代内核进行释义，并为Linux源代码Debug。</br>




---
## 各阶段实现步骤:
### 一阶段项目实现步骤:
1.内核释义：即对内核中的代码进行专业化的解释，追求语言简洁，和专业化拓展。</br>
2.伪算法规划：使用伪算法（半语言代码方式）进行大致框架搭建，从第一步中的代码释义中抽取最核心的步骤。</br>
3.算法初解：从伪算法中得出的核心进行更细致的抽象，将其抽象到图标中，使有基础的人们能明白其要义。</br>
4.大众推广：用通俗的语言对上述的抽象出的算法与理由进行创造，可讲故事、类比等各种方式，达到使其行业的人们也能读懂的程度。</br>

### 二阶段项目实现步骤:
1.大致构思：用通俗的语言，或者极易的方式，即普罗大众都能理解的方式进行解释与创造。</br>
2.算法初解：使用人类语言或结构图设计回溯关于某一核心部件/个性化层面的目的与最终结果，并提出具有一定逻辑的简要实现方式。</br>
3.伪算法规划：使用伪算法（半语言代码方式）进行大致规划与设计，即拥有一定的专业性.</br>
4.代码实现：用C、ASM进行实现，通过各种方式将该算法用语言实现。</br>
5.测试控制：切换内核，通过观察系统调度判断内核修改是否成功。</br>

---
### 项目号召：
此项目在初期由于笔者个人限制，导致很多专业技术方向出现偏差，但设立该项目的初衷并未发生许多改变。</br>

- 由于是操作系统项目，所以最先考虑的就是该方向，如果您对操作系统感兴趣可以毫不犹豫的加入该项目，因为它将会尽可能地展示Linux内核的各个功能，且在此基础尽量添加或者修改内核，从代码的角度直接入手，并希望这些更改在将来能应用到该行业和其他行业中。</br>

- 其次由于该项目直接面向代码，所以如果您对查阅代码感兴趣，可以翻阅它，Linux中的代码复杂与抽象程度是相当高深的；在此您将了解到目前主流的系统的基础，在一定程度上可加深您对计算机语言的理解；由于语言在很多方面是相通的，所以您对C或者汇编语言的加深同时，对您的主攻技术栈或许有帮助。</br>

- 再者，如果您是研究算法的技术人员，也可以考虑加入我们，在这里您将能遇到操作系统中精妙绝伦的算法设计，或者遇到脑洞大开，并无大用却有趣的算法构思，并浏览代码实现或者伪代码实现的奇怪的算法，并观察它们在操作系统中会有怎样的表现。</br>

- 最后，如若您只是闲来无事，也可以加入我们，在这里您将能看到最直白的算法描述与最质朴的实现方式，对您的专业与技术无任何要求。</br>

- 无论您是否在计算机行业，只要对这个信息世界的运行的基础感兴趣，都欢迎！</br>
