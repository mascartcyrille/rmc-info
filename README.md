# rmc-info
Random Matrix Compression INterpolation FunctiOn

# Installation
### Command line
- Clone the repository using `git clone https://github.com/mascartcyrille/rmc-info.git`

# Maintenance
### Command line
- Run `git status` to know the names of the files that have been modified/are new
- Run `git add file_names` to add the files for the next commit
- Run `git commit -m "Commit message"` to commit the previously added files. The Commit message summarizes the files that are added to the commit save.
- Run `git pull` for getting remote modifications to the files. Solve the eventual merge conflicts that appear.
- Run `git push` to put your modifications on the remote server.

# Class diagrams

```mermaid
classDiagram
      simpleMatch o-- rng
      rmcInfo o-- simpleMatch
      rmcInfo o-- matrixLoader
      rmcInfo o-- matrix
      rmcInfo o-- compressedMatrix
      matrixLoader -- matrix
      class rmcInfo{
          -int size
          -string path
          -rng rng
          -matrix matrix
          -matrixLoader matrixLoader
          -double currentTreshold
          -int[] seed
          +main()
      }
      class rng{
          -int[] state
          -int[] seed
          +generate()
          +getState()
          +setSeed()
      }
      class simpleMatch{
          -deque<double> currentRandomSequence
          -deque<int[]> states
          +match(matrix&, int, int)
          +initDeques(int)
          +shift()
      }
      class matrix{
          +vector<int>
          +run()
          +initMatrix(int size)
      }
      class matrixLoader{
          +readFile(matrix *, string)
      }
      class compressedMatrix{
          +vector<int[]> compressedMatrix
      }
```