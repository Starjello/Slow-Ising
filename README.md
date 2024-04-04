# Slow-Ising
My Ising model is running slowly

* Run a profiler to fine where it is slow (note on windows replace slow with slow.exe

      g++ -pg -o slow IsingModel.cpp IsingModelMain.cpp

      ./slow
  
      gprof slow gmon.out > analysis.txt* Fix the bug
* Commit the change

      git commit -a -m "PUT A MESSAGE HERE"
* Push the change

      git push
* Then create a pull request to the parent fork (do this on github)
