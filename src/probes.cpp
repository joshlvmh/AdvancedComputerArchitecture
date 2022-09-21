#include "probes.h"

string findProbes(int i, vector<vector<int> >* c_prob, vector<vector<int> >* c_comp)
{
  string probe = "";
  bool first = true;
  vector<int> probes;
  for (unsigned int j = 0; j < (*c_prob).size(); j++)
  {
    if ((*c_prob)[j][1] == (*c_comp)[i][7])
    {
      if (first)
      {
        probe = to_string((*c_prob)[j][0]) + "," + to_string((*c_prob)[j][1]);
        first = false;
        probes.push_back((*c_prob)[j][0]);
      }
      else if (find(probes.begin(), probes.end(), (*c_prob)[j][0]) == probes.end())
      {
        probe = probe + ":" + to_string((*c_prob)[j][0]) + "," + to_string((*c_prob)[j][1]);
      }
    }
  }
  return probe;
}

void trace_file(vector<vector<int> >* c_comp, vector<vector<int> >* c_prob, t_instr* instructions, vector<string>* all_instrs, int tics)
{
  ofstream trace;
  ofstream probe;
  stringstream pc;
  string line;
  trace.open("trace.out");
  probe.open("probe.out");

  for(int i = 0; i < tics; i++)
  {
    bool probe_found = false;
    string line;
    for (unsigned int k = 0; k < (*c_prob).size(); k++)
    {
      if ((*c_prob)[k][0] == i)
      {
        string event;
        if (probe_found) event = ":";
        event = event + to_string((*c_prob)[k][2]) + "," + to_string((*c_prob)[k][1]);
        probe_found = true;
        probe << event;
      }
    }
    if (!probe_found) probe << "-";
    probe << "\n";
  }

  std::string x = "-1", y = "0";

  for(unsigned int i = 0; i < (*c_comp).size(); i++)
  {
    line = to_string((*c_comp)[i][0]) + ":" + to_string((*c_comp)[i][1]) + ":" + to_string((*c_comp)[i][2]);
    line = line + ":" + to_string((*c_comp)[i][3]) + ":" + to_string((*c_comp)[i][4]);
    size_t pos;
    while ((pos = line.find(x)) != std::string::npos) {
      line.replace(pos, 2, y);
    }
    stringstream pc;
    pc << hex << (*c_comp)[i][5];
    line = line + ":" + "0x" + pc.str() + ":";
    line = line + to_string((*c_comp)[i][6]) + ":" + to_string((*c_comp)[i][7]) + ":";
    line = line + (*all_instrs)[((*c_comp)[i][5])/4];
    trace << line << "\n";
  }
  trace.close();
  probe.close();
}
