Orga-computador2
================
This repository stores all the practical works developed during the subject Computer Organization II. This includes:
  Note: all the projects were tested with valgrind for leaks when memory dynamic was used. Also measurements were taken using   the time-stamp-counter register in the cpu for performance analysis(ie. TP2 using SIMD)

  * TP1: A linked-list implementation which holds another linked-list in its nodes. The goal of this model was being able
  * to perform certain operations such as filter and merge the child linked-list of each node of the main linked-list using      several functions to make decisions about wheter to filter or merge a given node. This implementation was first developed     in C and it was ported to a x64 Intel ASM version. 

  * TP2: Several video(or image) filters were applied to input files, using openCV the raw input was converted to a single image of a vector of images, all of them consisted of a matriz of pixels. In order to apply the filters we modified this  raster images and save them back into a output file. In this context, a lot of filters make the same arithmetic operation over and over again, and using Intel SIMD technology, we could really improve the performance. First the filters were developed in C, and then ported to ASM x64 using SIMD XMM registers in order to parallellize the operations, depending on which algorithm were we coding, more or less performance gain was obtained. Attached to the source, is a report(in spanish) describing all the experiments done and its results. Aditional experiments were made. For example: it was tested the branch prediction, it was compared the performance between the C versions compiling with -O0, -O1, -O2, -O3 and the ASM version using SIMD, etc.
  
  * TP3: This project was basically, given a bootloader, making a kernel which takes the machine from real mode, setting up the GDT, jumping into protected mode, setting up a basic Exceptions and Interrupcions handling(hardware ans syscalls), implementing an arbitrary virtual memory scheme, a basic MMU which maps and unmaps memory pages between virtual and physical memory, using the TSS Intel technology, were coded 8 tasks, each of which makes use of 3 syscalls provided in the kernel.
The tasks were part of a "game" in which each task must destroy the others using the syscalls. A round robin scheduler was also coded to provide the multitasking enviroment, tasks exception handling and recovery, etc. The detailed information can be read in the pdfs in this folder.
Below are described the syscalls
  * Read kernel memory area: this was intended for the tasks to take a glimpse of the pagination scheme(multi-level tables) and for example, discover where in the memory map where the other tasks and attack them.
  * Move in "sea" memory: this syscall allowed a process to copy its pages to another part of the physical memory
  * Fire a byte bullet: this syscall allowed a task to write an arbitrary number of bytes stored in a buffer of its own to any part of the physical "sea" memory. This could be seen as "attacking" another tasks.

* Protection, memory areas(sea and ground, etc)
  Physical memory was divided in 2, "ground" memory, which was were the kernel stored all its structures and "sea" memory, which was were all the tasks were stored. Any attempt of a task to write into kernel "ground" memory would come into a General Protection exception, this was achieved using different segments in the GDT with different ring permissions. Tasks run in ring 3, and kernel run in ring 0.
  

Aditionally i've been part of another project as an extension of TP3. The information about that project is here:
https://github.com/Izikiel/intel_multicore

  
