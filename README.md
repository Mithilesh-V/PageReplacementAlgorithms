# Page Replacement Algorithms

### Video: https://drive.google.com/file/d/1weh224q3bVDFpHZHImuO3I1ApXMpAUEe/view?usp=sharing

## 1.     Write a program to simulate the Not-Recently-Used (NRU) Page Replacement Algorithm.

       Number of Virtual Pages
       Number of Page Frames Available (not used by page table)
       Number of References
       Page Referenced   Read or Write (0 or 1, respectively)
       Page Referenced   Read or Write (0 or 1, respectively)
                :			:

       For example:

          8	<= 8 virtual pages
          4	<= 4 page frames
          4	<= 4 referencs
          0 0	<= Page 0 is read from.
          1 1	<= Page 1 is written to.
          3 0	<= Page 3 is read from.
          4 1	<= Page 5 is written to.

       is valid input. There are 8 virtual pages (numbered 0, 1, ... 7),
       4 page frames (numbered 0, 1, 2, 3) in memory, 3 references listed: 
       page 0 is read from, page 1 is written to, page 3 is read from, and
       page 5 is written to.

       Maintain a page table as an array. Use page number as an index to
       the array. The array should include an R-bit, an M-bit, and a
       Present/Absent bit, as well as the frame currently mapped.

       1. When a page is referenced, R-bit is set to 1 and M-bit is set to 0.
       2. When a page is modified, R-bit is set to 1 and M-bit is set to 1.
       3. Assume that the R-bit is reset to 0 after a set of 6 pages have 
          been referenced. This is to simulate a reset after each clock 
          interrupt.

       Corresponding to the previous example, the page table could look
       like this way:

       1 0 1 0  <= Page 0: R-bit = 1, M-bit = 0, Present in Frame 0
       1 1 1 1  <= Page 1: R-bit = 1, M-bit = 1, Present in Frame 1
       0 0 0 0  <= Page 2: R-bit = 0, M-bit = 0, Absent from any frame
       1 0 1 2  <= Page 3: R-bit = 1, M-bit = 0, Present in Frame 2
       1 1 1 3  <= Page 4: R-bit = 1, M-bit = 1, Present in Frame 3
       0 0 0 0  <= Page 5: R-bit = 0, M-bit = 0, Absent from any frame
       0 0 0 0  <= Page 6: R-bit = 0, M-bit = 0, Absent from any frame
       0 0 0 0  <= Page 7: R-bit = 0, M-bit = 0, Absent from any frame
 
       The first row indicates the virtual page 0 is mapped to the physical
       page 0. The page has been referenced and is present in the physical
       memory.

       The output from the simulator should indicate whenever a fault
       occurs, and which page has been evicted (if one is evicted).
       Always select the smallest numbered page in a class to be evicted.
       In addition, the output should include the total number of
       faults that occurred. 
             
       The possible output would look something like:

                    : : 
            Fault: no page evicted, page 0 brought in to memory.
            Fault: page 2 evicted, page 3 brought in to memory.
                    : :
            Total number of faults is 23.

       For example, if the input data is

          8	<= 8 virtual pages
          4	<= 4 page frames
          5	<= 5 referencs
          0 0	<= Page 0 is read from.
          1 1	<= Page 1 is written to.
          3 0	<= Page 3 is read from.
          4 1	<= Page 4 is written to.
          5 1	<= Page 5 is written to.

       
       The output would be:

       Page Fault: no page evicted, page 0 brought in to memory.
       Page Fault: no page evicted, page 1 brought in to memory.
       Page Fault: no page evicted, page 3 brought in to memory.
       Page Fault: no page evicted, page 4 brought in to memory.
       Page Fault: page 0 evicted, page 5 brought in to memory.

       Total number of faults is 5.

##    2. Write a program to simulate the Clock Page Replacement Algorithm. 

       For example, if the input data is

          8	<= 8 virtual pages
          4	<= 4 page frames
          10	<= 10 referencs
          0 	<= Page 0 is referenced.
          1 	<= Page 1 is referenced.
          3 	<= Page 3 is referenced.
          4 	<= Page 4 is referenced.
          5 	<= Page 5 is referenced.
          0 	<= Page 0 is referenced.
          3 	<= Page 3 is referenced.
          4 	<= Page 4 is referenced.
          2 	<= Page 2 is referenced.
          6 	<= Page 6 is referenced.


       The output would be:

       Page Fault: no page evicted, page 0 brought in to memory.
       Page Fault: no page evicted, page 1 brought in to memory.
       Page Fault: no page evicted, page 3 brought in to memory.
       Page Fault: no page evicted, page 4 brought in to memory.
       Page Fault: page 0 evicted, page 5 brought in to memory.
       Page Fault: page 1 evicted, page 0 brought in to memory.
       Page Fault: page 5 evicted, page 2 brought in to memory.
       Page Fault: page 0 evicted, page 6 brought in to memory.

       Total number of faults is 8.

  

#    3. Write a program to simulate the Aging Page Replacement Algorithm.

       Number of Page Frames Available
       Number of bits of the counter
       Number of clock ticks
       R bits for pages 0 - (Number of Page Frames - 1), clock tick 0
       R bits for pages 0 - (Number of Page Frames - 1), clock tick 1
                :			:

       For example:

          6		<= 6 Page frames
          8		<= 8-bit coutner
          5		<= 5 clock ticks
          1 0 1 0 1 1	<= Reference at the 1st clock tick
          1 1 0 0 1 0	<= Reference at the 2nd clock tick
          1 1 0 1 0 1	<= Reference at the 3rd clock tick
          1 0 0 0 1 0	<= Reference at the 4th clock tick
          0 1 1 0 0 0	<= Reference at the 5th clock tick

       is valid input. There are 4 page frames (numbered 0, 1, 2, 3) in 
       memory, 5 clock ticks, and an 8-bit counter. At first clock tick, 
       R bits are 1, 0, 1, 0, 1, and 1, repectivly (page 0 is 1, page 1 
       is 0, page 2 is 1, etc.). 

       The output from the simulator should indicate the counters in the
       last clock tick and which page frame is going to be evicted.


       The possible output would look something like:

       The 8-bit counters for 6 page frames after 5 clock ticks are:

       01111000
       10110000
       10001000
       00100000
       01011000
       00101000

       The page frame 3 will be evicted.

       The 4-bit counters for 6 page frames after 5 clock ticks are:
                    : :


    Note: In this assignment you can assume at maximum there are 16 
          pages or frames in the system. The maximum length of counters
          is 16 bits.



