# Your solution for Cities task
This is your repository for Cities task solution.

Available cities are present in `input.txt`. Results should be written into `output.txt` file.

All the code for reading/writing files is already present, so you can focus on solving the task.

Place your code in `combine_cities` method of `main.cpp` file:
```
vector<string> combine_cities(vector<string> available_cities) {
  // TODO replace with your solution!
  return vector<string>();
}
 ```

Run `main` method in `main.cpp` file and verify that `output.txt` file is updated:

### To submit your solution just push your code to this repo 😎

Detailed description of the task is [here](https://betterhire.me/tasks/CITIES/description)

Happy coding! 😉

# OK, lets go!
## Thank [betterhire](https://github.com/BetterHireMe), for this interesting task.
### It's time to remake it to achieve a better result.
In new solution I used the ant search algorithm with some modifications: 
- the lower and upper limits of pheromones on the edges of the graph
- elite ants to increase the number of pheromones on the edges of a better way

The one of the main differences is the possibility of the ant returning to the previous vertex of the graph and its passage along other available edges(like a depth-first search algorithm)

Also a key difference is the change in the coefficient Beta if the path improvement has not occurred after a certain number of iterations.

As a result I achieved a 16705 characters in 148346 iterations (WOW) spending only 17584.4 seconds. 
It should be noted that the maximum result that was achieved in the process of experiments 16715 characters(please see the file `best_way.txt`).

Therefore, I believe that my algorithm is the best of the proposed solutions [here](https://betterhire.me/tasks/CITIES/description).

