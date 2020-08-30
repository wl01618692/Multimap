#include <utility>
#include <vector>
#include <iostream>
#include <algorithm>
#include "../LLRB-Multimap/multimap.h"
#include <fstream>
class Task{
 public:
  // Constructor
  Task(char name, int tick_value, int duration);
  Task() = default;

  // Compare operator
  bool operator<(const Task& rhs) const;

  // Overloading << operator
  friend std::ostream& operator<<(std::ostream& os, const Task& rhs);

  char name_;
  unsigned int tick_start_time;
  unsigned int tick_duration;
  unsigned int vruntime_;
  unsigned int runtime_;
};

// This is a constructor for Task loading three arguments
Task::Task(char name, int tick_value, int duration):
    name_(name), tick_start_time(tick_value),
    tick_duration(duration), vruntime_(0), runtime_(0) {}

// This function overloads <<operator for Object Task for debugging and output
std::ostream &operator<<(std::ostream &os, const Task &rhs) {
  os << rhs.name_ << " " << rhs.tick_start_time << " " << rhs.tick_duration;
  os << std::endl;
  return os;
}

// This function compares *this and *rhs by their start time
bool Task::operator<(const Task& rhs) const {
  return this->tick_start_time < rhs.tick_start_time;
}

// This function compares *this and *rhs by their name
struct TaskGreaterName {
  bool operator()( const Task& Lhs, const Task& Rhs ) const {
    return Lhs.name_ < Rhs.name_;
  }
};


class CFS{
 public:
  // Constructor
  CFS(const std::vector<Task *>& vector);

  // Start running the Completely Fair Scheduler with a give schedule
  void Run();
  bool CheckRunTime();
 private:
  unsigned int min_vruntime = 0;
  Multimap<unsigned int, Task*> timeline_;
  std::vector<Task*> current_running_task_vector;
  std::vector<Task*> task_vector;
  Task* current_running_task;
  unsigned int tick_ = 0;
  unsigned long current_running_task_number = 0;
  unsigned long number_of_task_completed_ = 0;
  bool InTask();
};

// This function debug the schedule by <<operator
void PrintTaskVector(const std::vector<Task*>& schedule) {
  for (const auto &item : schedule) {
    std::cout << *item;
  }
}

void CFS::Run() {
  // If the all the tasks are completed , then we stop running
  std::vector<Task *> copy_vector = task_vector;
  PrintTaskVector(task_vector);
  while (number_of_task_completed_ != task_vector.size()) {
    // If there are tasks that should be launched at the current tick value,
    // we add them to the timeline of tasks.
    // The idea is go through the copy vector
    // and check if the Task has the same start time as
    // current tick. If it has, then remove it from the copy vector.
    std::vector<Task*> output_vector;
    for (unsigned long i = 0; i < task_vector.size(); ++i) {
      if (copy_vector[0]->tick_start_time == tick_ && !copy_vector.empty()) {
        copy_vector.front()->vruntime_ = min_vruntime;
        current_running_task_vector.emplace_back(copy_vector[0]);
        output_vector.emplace_back(copy_vector[0]);
        current_running_task = copy_vector[0];
        timeline_.Insert(min_vruntime, copy_vector[0]);
        ++current_running_task_number;
        // Delete it from the copy vector
        copy_vector.erase(copy_vector.begin());
      }
      // If copy schedule is empty or the front doesnt equal to tick, then stop
      if (copy_vector.empty() ||
      copy_vector[0]->tick_start_time != tick_) {
        break;
      }
    }
    if (current_running_task_number == 0) {
      std::cout << tick_ << " " << "[" << 0 << "]: _" << std::endl;
      ++tick_;
      continue;
    }

    if (!InTask()) {
      std::cout << tick_ << " [" << current_running_task_number << "]: "
                << current_running_task->name_ << "*" << std::endl;
    } else {
      std::cout << tick_ << " [" << current_running_task_number << "]: "
                << current_running_task->name_ << std::endl;
    }


    if (CheckRunTime()) {
      min_vruntime = timeline_.Get(timeline_.Min())->vruntime_ + 1;
    }
    current_running_task->vruntime_++;
    current_running_task->runtime_++;
    ++tick_;
    if (current_running_task->tick_duration == current_running_task->runtime_) {
      ++number_of_task_completed_;
      timeline_.Remove(current_running_task->tick_start_time);
    }
  }
}

CFS::CFS(const std::vector<Task *>& vector) {
  for (const auto & task : vector) {
    task_vector.emplace_back(task);
  }
  // PrintTaskVector(schedule);
}

bool CFS::CheckRunTime() {
  for (auto & i : current_running_task_vector) {
    if (i->runtime_ <= min_vruntime) {
      return false;
    }
  }
  return true;
}

bool CFS::InTask() {
  for (const auto &item : current_running_task_vector) {
    if (current_running_task == item) {
      return true;
    }
  }
  return false;
}

// This function checks the number of command line argument
bool CheckCommandLine(const int &argc, char** file_name) {
  if (argc != 2) {
    std::cerr << "Usage: " << file_name[0] << " <task_file.dat>";
    return false;
  }
  return true;
}

// This function checks if the file name is valid and can be opened
bool CheckFile(char** file_name) {
  std::fstream task_file(file_name[1]);
  if (!task_file.good()) {
    std::cerr << "Error: cannot open file " << file_name[1] << std::endl;
    return false;
  }
  return true;
}

// This function loads the file and store it in a vector of Task pointer
void LoadFile(char** file_name, std::vector<Task*> &schedule) {
  // Variable Declaration
  std::ifstream task_file(file_name[1]);
  char name;
  int tick_value;
  int duration;
  // Read three at once with >> overload
  while (task_file >> name >> tick_value >> duration) {
    Task* my_task = new Task{name, tick_value, duration};
    schedule.emplace_back(my_task);
  }
}

void SortSchedule(std::vector<Task*>& schedule){
  std::vector<Task> copy_schedule;
  copy_schedule.reserve(schedule.size());
  for (const auto &item : schedule) {
    copy_schedule.emplace_back(*item);
  }
  // We sort the copy vector with the overload <operator for Task
  std::sort(copy_schedule.begin(), copy_schedule.end());
  std::sort(copy_schedule.begin(), copy_schedule.end(),TaskGreaterName());
  // Now we rearrange the schedule vector into sorted order by copy vector
  schedule.clear();
  for (auto & i : copy_schedule) {
    schedule.emplace_back(&i);
  }
}

void Delete(std::vector<Task *> vector) {
  for (auto &item : vector) {
    delete(item);
  }
}

int main(int argc, char** argv) {
  // Verify the command line format and file condition
  if (!CheckCommandLine(argc, argv) || !CheckFile(argv)) {
    exit(1);
  }
  // Variable declaration
  std::vector<Task*> schedule;

  // Load the file and sort the schedule
  LoadFile(argv, schedule);
  // PrintTaskVector(schedule);
  SortSchedule(schedule);

  // Debugging for loading schedule
  // PrintTaskVector(schedule);

  // Run the scheduler
  CFS scheduler(schedule);
  // My array seems to have gibberish values
  // after entering this function and I cant find the bug
  scheduler.Run();
  // Delete the array dynamically
  Delete(schedule);
  return 0;
}
