CSL2020 | Data Structures and Algorithms 

Instructor: - Dip Sankar Banerjee Sir

Project:
Topic: - Navigating Airports Using Graph Algorithms
Mentor TA's : - Jainan Nareshkumar Tandel (M23CSA010) 

Team : -
1. Binay Suman (B22CS019)

***This File Contains Instructions on how to use the codes.***
Link to the Github Repo: - “ https://github.com/binay4/DSAproject ”

what our submission contains: -
A) 2 csv files
    1. airports_final.csv : Contains all the airport information which are used as vertex.
    2. routes_final.csv : Contains all the routes data which is used for making edges.

B) 3 'c++' code files
    1. dijkstra.cpp
    2. Bellman.cpp
    3. floyd.cpp

C) Other File
    1. visualization.svg : - Shows the visualization for the made graphs.
    2. Distance_matrix.txt : - Contains the output matrix given by floyd algorithm.
    3. timecomplexity.xlxs : - Contains all the results and graphs.
    4. B22CS019_prjct.pptx : - Presentation.
    5. B22CS019_prjct.pdf : - Presentation.
    PPT Link: - B22CS019_prjct
**How to Use**
1. Make sure that two csv files and 3 code files are placed in same folder.
2. open “cmd” in the folder where all the files are placed.
3. Use the following commands 
      To Compile: -
          Dijkstra: - "g++ -w dijkstra.cpp -o dijkstra"
          Bellman-Ford: - "g++ -w Bellman.cpp -o bellman"
          Floyd: - "g++ -w floyd.cpp -o floyd"
        
        make sure to include "-w" while compiling to avoid any errors.
          
      To Execute: -
          Dijkstra: -"./dijkstra"
          Floyd: -"./floyd"
          Bellman-Ford: -"./bellman"
        
4. If you are using VS code you can run the following commands in the terminal from VS Code
        while executing the program 
        dijkstra.c: -"./dijkstra"
        Bellman.c: -"./bellman"
        floyd.c: - "./floyd"

5. After Executing, the program will ask for input and output (IATA codes). For that you can refer to the CSV files (airports file) that contains information of every airport where you can find IATA codes, there is also a routes csv file where only the IATA codes are mentioned of source and destination airports.
6. There is a SVG File that was generated on this dataset which visualizes the data along with the weights, you can refer to that to verify the output.
