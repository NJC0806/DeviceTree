		
										设备树学习笔记
										
-----------------------------------------------------------------------------------------------------------

1、设备的来源(device tree source)	
									
	Linux内核从3.x开始引入设备树的概念，用于实现驱动代码与设备信息相分离。在设备树出现以前，所有关于
设备的具体信息都要写在驱动里，一旦外围设备变化，驱动代码就要重写。引入了设备树之后，驱动代码只负责处
理驱动的逻辑，而关于设备的具体信息存放到设备树文件中，这样，如果只是硬件接口信息的变化而没有驱动逻辑
的变化，驱动开发者只需要修改设备树文件信息，不需要改写驱动代码。
	基于同样的软件分层设计的思想，由于一个SoC可能对应多个machine，如果每个machine的设备树都写成一个完
全独立的.dts文件，那么势必相当一些.dts文件有重复的部分，为了解决这个问题，Linux设备树目录把一个SoC公用
的部分或者多个machine共同的部分提炼为相应的.dtsi文件。这样每个.dts就只有自己差异的部分，公有的部分只需
要"include"相应的.dtsi文件, 这样就是整个设备树的管理更加有序。

-----------------------------------------------------------------------------------------------------------

2、设备树的功能

	1、Device Tree可以描述的信息包括CPU的数量和类别、内存基地址和大小、总线和桥、外设连接、中断控制器和中断
	   使用情况、GPIO控制器和GPIO使用情况、Clock控制器和Clock使用情况。
	2、设备树信息被保存在一个ASCII 文本文件中,适合人类的阅读习惯,类似于xml文件,在ARM Linux中,一个.dts文件对
	   应一个ARM的machine放置在内核的arch/arm/boot/dts/目录
	3、设备树是一种数据结构,用于描述设备信息的语言,具体而言, 是用于操作系统中描述硬件,使得不需要对设备的信
	   息进行硬编码(hard code)
	4、Device Tree由一系列被命名的结点(node)和属性(property)组成,而结点本身可包含子结点。所谓属性，其实
	   就是成对出现的name和value(键值对)
	5、设备树源文件dts被编译成dtb二进制文件,在bootloader运行时传递给内核,内核对其进行解析展开(Flattened)从而
	   产生一个硬件设备的拓扑图有了这个拓扑图,在编程的过程中可以直接通过系统提供的接口获取到设备树中的节点和
	   属性信息。
   
-----------------------------------------------------------------------------------------------------------

3、名词解释
	DT:device tree
	FDT:flattened device tree
	OF:open firmware//打开固件
	DTS:device tree source
	DTSI:device tree source include 
	DTB:device tree blob 
	DTC:device tree compiler

-----------------------------------------------------------------------------------------------------------

4、设备树的语法

	节点node
	节点名称:每个节点必须有一一个"<名称> [@<设备地址>]”形式的名字
	<名称>:就是一一个不超过31位的简单ASCII字符串,节点的命名应该根据它所体现的是什么样的设备。比如一个3com以
	太网适配器的节点就应该命名为ethernet ,而不应该是3com509.
	
	<设备地址>:用来访问该设备的主地址,并且该地址也在节点的reg属性中列出,同级节点命名必须是唯一的,但只要地址
	不同,多个节点也可以使用一样的通用名称，当然设备地址也是可选的,可以有也可以没有树中每个表示个设备的节点
	都需要一个 compatible属性,此处的设备地址不是用来匹配的，而仅仅是用来区分不同的设备节点的。属性property
	其实就是简单的键(名字)-值对,它的值可以为空或者包含-一个任意字节流。虽然数据类型并没有编码进数据结构,但在
	设备树源文件中任有几个基本的数据表示形式:
	
	文本字符串(无结束符)可以用双引号表示:
			string-property = "a string"
	cells是32位无符号整数,用尖括号限定:
			cell-property = <0xbeef 123 0xabcd1234>
	二进制数据用方括号限定:
			binary-property= [01 23 45 67];//必须是偶数，不可以是123这样的奇数
							  //01 23 45 97实际上是连续的
	 不同表示形式的数据可以使用逗号连在一起:
			mixed-property= "a string", [01 23 45 67], <0x12345678>;
	 逗号也可用于创建字符串列表:
			string-list = "red fish", "blue fish";

-----------------------------------------------------------------------------------------------------------

5、设备树模板

/{																//根节点
	node1{														//子节点1
		a-string-property="a string";							//属性可以为字符串(自定义的属性)
		a-string-list-property="fist string","second string";	//属性可以为字符串列表
		a-byte-data-property=[0x01 0x23 0x24 0x56];				//属性可以为二进制数组
		child-node1{											//子子节点1
					fis-child-property;							//属性可以为空，只是一个标志位
					second-child-property=<1>;					//属性可以基本的数据类型
					a-string-property="hello world";
		};
		child-node2{											//子子节点1
					……
					……
		};
	};
	node2{														//子节点2
			an-empty-property;
			a-cell-property=<1 2 3>;							//属性可以为整数数组(32位uint32)
			child-node1{
						……
			};
	};
};

-----------------------------------------------------------------------------------------------------------

6、常见属性

	设备树中有很多定义好的属性例如
								compatible
								reg
								label
								gpios
								pwms
								status
		也可以位自定义的属性 a-string-property(上述模板中全部是自定义的)
		
	6.1、Compatible属性
		指定了系统的名称，是一个字符串的列表，实际在代码中可以用于进行匹配，当前你选择的是哪一个及机器，它包
	括一个<制造商>,<型号>形式的字符串，重要的是要指定一个确切的设备且包括制造商的名字，以避免命名空间的冲突。
	/{
		Compatible =”acme,coyotes-revenge”
	}；
	
	6.2、#address-cell和#size-cell属性
		#address-cel=<1>表示address字段的长度为
		#size-cell=<1>表示length字段的长度为1
	
		external-bus{
			#address-cel=<2>//针对子节点的reg,不针对子子节点
			#size-cell=<1>;//针对子节点的reg，不针对子子节点
			ethernet@0,0{
				compatible=”smc,smc91c111”;
				reg=<0 0 0x1000>;//地址占2个cells，长度占1个cells
				interrupt =<5 2>;
			};
		};

	6.3、reg属性
		reg的组织形式为reg=<address1 length1 [address2 lengtth2]  [address3 lengtth3] ...>其中的每一组address 
	length 表明设备使用的一个地址范围.
		/{
			compatible=”acme,coyoters-revenge”
			#address-cel=<1>;
			#size-cell=<1>;
			serial@101f2000{
					 compatible=”aem,pl011”;
					 reg=<0x101f2000 0x1000>;
					 Interrupts=<2 0>;
			};
		};


	6.4、中断信息属性
		interupt-controller一个空的属性定义节点作为一个接收中断信号的设备，#interrupt-cells这是一个中断控制
	节点的属性，它声明了该中断控制器的中断指示符中cell的个数(类似#address-cells)。
		interrupt-parent 这是义格式设备几点的属性，包含一个指向该设备连接的中断控制器的phandle(指向或者可以引
	用&)那些没有interruot-parent的节点则是从他们的父节点中继承该属性。
		interrupts一个设备节点属性,包含一个中断指示符的列表，对应于该设备上的每个中断输出信号.
	/{
	    compatible=”acme,coyotes-revenge”;
		#address-cel=<1>;
		#size-cell=<1>;
		interrput-parent=<&intc>//子节点引用的是另外一个节点的所有信息
		serial@101f2000{
				 compatible=”aem,pl011”;
				 reg=<0x101f2000 0x1000>;
				 interrupts=<1 0>;
			};
		intc:interrupt-controller@10140000{  //intc:表示可以外部引用整个节点
					compatible=”arm,pl190”;
					reg=<0x10140000 0x1000>;
					interrupt-controller;
					#interrput-cells=<2>;
			};
	};
	
	对于arm架构,标志为具体含义Documentation/devicetree/bindings/arm/gic.txt
	
	ARM SMP cores are often associated with a GIC, providing per processor interrupts (PPI), shared processor
	interrupts (SPI) and software generated interrupts (SGI).
	The 1st cell is the interrupt type; 0 for SPI interrupts, 1 for PPI interrupts.//0表示共享中断，1表示独占中断
	The 2nd cell contains the interrupt number for the interrupt type.SPI interrupts are in the range [0-987].  
	PPI interrupts are in the range [0-15].
	The 3rd cell is the flags, encoded as follows:
	bits[3:0] trigger type and level flags.
		1 = low-to-high edge triggered//上升沿触发
		2 = high-to-low edge triggered//下降沿触发
		4 = active high level-sensitive//高电平触发
		8 = active low level-sensitive//低电平触发
	bits[15:8] PPI interrupt cpu mask.  Each bit corresponds to each of the 8 possible cpus attached to the GIC. 
	A bit set to '1' indicatedthe interrupt is wired to that CPU.  Only valid for PPI interrupts.
	
-----------------------------------------------------------------------------------------------------------

7、在驱动中如何解析设备树属性

	可以在/proc/device-tree中找到设备树中的节点以及属性内核提供获得设备树的数据的API全部在driver/of/目录下  
	
	of_find_node_by_path(/nodename);//找节点
	if_find_property();//找属性(所有的属性的值都存在这个函数对应的结构体中)
	of_device_compatible();//判断是否定义compatible节点
	of_property_read_u32_array()读取到属性中的整数数组
	of_property_read_string_index()//读取属性中的字符串数组
	if(of_find_property(np,”xxx”，NULL))
	{
		FALG=1;
		//在设备树中定义了一个值为空的属性，可以在驱动中用来设置标志位
	}//属性为空，实际可以用于设定标志位

	interrupt-parent = <&gpio7>;//继承gpio7这一组的属性
	interrupts = <4 IRQ_TYPE_EDGE_FALLING>;
	//表示中断GPIO为gpio7_4
	irqnumber=irq_of_parse_and_map();//获取中断编号

