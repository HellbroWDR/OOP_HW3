#include <bits/stdc++.h>
#define x first
#define y second
#define pi 3.14159265
using namespace std;

class node;
class packet
{
private:
    node *des, *sou, *nhop, *lhop;
    double inpox, inpoy;
public:
    friend class node;
    void sdes(node *d)
    {
        des = d;
    }
    void ssou(node *s)
    {
        sou = s;
    }
    void snhop(node *hop)
    {
        nhop = hop;
    }
    void slhop(node *hop)
    {
        lhop = hop;
    }
    void sinpo(double x, double y)
    {
        inpox = x;
        inpoy = y;
    }
    node* glhop()
    {
        return lhop;
    }
    node* gnhop()
    {
        return nhop;
    }
    node* gdes()
    {
        return des;
    }
    node* gsou()
    {
        return sou;
    }
    double gipx()
    {
        return inpox;
    }
    double gipy()
    {
        return inpoy;
    }
};

class node
{
private:
    double des_x;
    double des_y;
    int num;
public:
    vector<int> neighbors_before;
    vector<node*> neighbors_after;
    queue<packet> storepacket;
    double get_desx()
    {
        return des_x;
    }
    double get_desy()
    {
        return des_y;
    }
    void set_des(double x, double y)
    {
        des_x = x;
        des_y = y;
        return ;
    }
    void snum(int n)
    {
        num = n;
    }
    int gnum()
    {
        return num;
    }
    bool checkqueue();
    node* getNextHop(packet p);
    void send(packet p)
    {
        for(int i = 0; i < neighbors_after.size(); i++)
        {
            node *tmp = neighbors_after[i];
            tmp->storepacket.push(p);
        }
    }
    bool JudgeCrossLine(packet p, node n1);
    bool SetInpoAndJudgeNextNode(packet &p, node *n1);
};

vector<node> graph;

int main()
{
    freopen("node2.txt", "r", stdin);
    //freopen("node333fuck.txt", "w", stdout);
    int node_sum, target_sum, edge_sum;
    cin >> node_sum;
    for(int i = 0; i < node_sum; i++)
    {
        int node_num;
        pair<double, double> node_des;
        cin >> node_num >> node_des.x >> node_des.y;
        node tmp;
        tmp.set_des(node_des.x, node_des.y);
        tmp.snum(node_num);
        graph.push_back(tmp);
    }
    for(int i = 0; i < node_sum; i++)
    {
        for(int j = 0; j < node_sum; j++)
        {
            if(i == j)
                continue;
            double dis = sqrt(pow(graph[i].get_desx()-graph[j].get_desx(),2.0)+pow(graph[i].get_desy()-graph[j].get_desy(),2.0));
            if(dis < 1)
                graph[i].neighbors_before.push_back(j);
        }
    }
    for(int i = 0; i < graph.size(); i++)
    {
        int flag[graph[i].neighbors_before.size()] = {0};
        for(int j = 0; j < graph[i].neighbors_before.size(); j++)
        {
            int now_nbor = graph[i].neighbors_before[j];
            double center_x = (graph[i].get_desx()+graph[now_nbor].get_desx())/2;
            double center_y = (graph[i].get_desy()+graph[now_nbor].get_desy())/2;
            double radius = sqrt(pow(graph[i].get_desx()-graph[now_nbor].get_desx(),2.0)+pow(graph[i].get_desy()-graph[now_nbor].get_desy(),2.0))/2;
            //半徑
            for(int k = 0; k < graph[i].neighbors_before.size(); k++)
            {
                if(k == j || flag[k])
                    continue;
                int now_other = graph[i].neighbors_before[k];
                double now_other_des_x = graph[now_other].get_desx();
                double now_other_des_y = graph[now_other].get_desy();
                double other_to_center = sqrt(pow(graph[now_other].get_desx()-center_x,2.0)+pow(graph[now_other].get_desy()-center_y,2.0));
                //另一點到圓心的距離
                if(other_to_center - radius < 1e-7)
                {
                    flag[j] = 1;
                    break;
                }
            }
        }

        for(int j = 0; j < graph[i].neighbors_before.size(); j++)
            if(!flag[j])
                graph[i].neighbors_after.push_back(&graph[graph[i].neighbors_before[j]]);
    }
    int des, sou, flow_sum;
    cin >>  flow_sum;
    for(int i = 0; i < flow_sum; i++)
    {
        cin >> sou >> des;
        packet p;
        p.sdes(&graph[des]);
        p.ssou(&graph[sou]);
        p.snhop(&graph[sou]);
        p.slhop(&graph[des]);
        p.sinpo(graph[sou].get_desx(), graph[sou].get_desy());
        graph[sou].storepacket.push(p);
        node* tmp = &graph[sou];
        cout << sou << " ";
        while(tmp->gnum() != graph[des].gnum())
        {
            if(tmp->checkqueue())
            {
                p = tmp->storepacket.front();
                tmp->storepacket.pop();
                tmp = tmp->getNextHop(p);
            }
        }
        cout << endl;
        for(int j = 0; j < graph.size(); j++)
            while(!graph[j].storepacket.empty())
                graph[j].storepacket.pop();
    }
    return 0;
}

bool node::checkqueue()
{
    packet tmp;
    if(storepacket.empty())
    {
        return false;
    }
    else
    {
        tmp = storepacket.front();
    }
    if(tmp.gnhop()->gnum() == gnum())
    {
        return true;
    }
    else
    {
        storepacket.pop();
        return false;
    }
}

node* node::getNextHop(packet p)
{
    double mangle = 361;
    node *mnode = NULL;
    double m = (p.gsou()->get_desy()-p.gdes()->get_desy())/(p.gsou()->get_desx()-p.gdes()->get_desx());
    double a_x, a_y;
    a_x = p.lhop->get_desx()-get_desx(), a_y = p.lhop->get_desy()-get_desy();
    double a = sqrt(pow(a_x,2)+pow(a_y,2));
    for(int i = 0; i < neighbors_after.size(); i++)
    {
        node *tmp = neighbors_after[i];
        if(!JudgeCrossLine(p, *tmp))
        {
            if(!SetInpoAndJudgeNextNode(p, tmp))
            {
                continue;
            }
        }
        double b_x = tmp->get_desx()-get_desx(), b_y = tmp->get_desy()-get_desy();
        double b = sqrt(pow(b_x,2)+pow(b_y,2));
        double adotb = a_x*b_x+a_y*b_y;
        double result = acos(adotb/(a*b))*180.0/pi;
        double cro = a_x*b_y-a_y*b_x;
        if(cro > 0)
            result = 360-result;
        if(tmp == p.glhop() && tmp != p.gdes())
            result = 360;
        if(result < mangle)
        {
            mnode = tmp;
            mangle = result;
        }
    }
    cout << mnode->gnum() << " ";
    p.slhop(p.gnhop());
    p.snhop(mnode);
    send(p);
    return mnode;
}

bool node::JudgeCrossLine(packet p, node n1)
{
    double slope = (p.gsou()->get_desy()-p.gdes()->get_desy())/(p.gsou()->get_desx()-p.gdes()->get_desx());
    double d1 = (get_desy()-p.gsou()->get_desy())-slope*(get_desx()-p.gsou()->get_desx());
    double d2 = (n1.get_desy()-p.gsou()->get_desy())-slope*(n1.get_desx()-p.gsou()->get_desx());
    if(fabs(d1) < 1e-7 || fabs(d2) < 1e-7)
        return true;
    else if(d1*d2 > 0)
        return true;
    else
        return false;
}

bool node::SetInpoAndJudgeNextNode(packet &p, node *n1)
{
    double x1 = p.gsou()->get_desx(), x2 = p.gdes()->get_desx(), x3 = get_desx(), x4 = n1->get_desx();
    double y1 = p.gsou()->get_desy(), y2 = p.gdes()->get_desy(), y3 = get_desy(), y4 = n1->get_desy();
    double tinpox = ((x1*y2-y1*x2)*(x3-x4)-(x1-x2)*(x3*y4-y3*x4))/((x1-x2)*(y3-y4)-(y1-y2)*(x3-x4));
    double tinpoy = ((x1*y2-y1*x2)*(y3-y4)-(y1-y2)*(x3*y4-y3*x4))/((x1-x2)*(y3-y4)-(y1-y2)*(x3-x4));
    double l1 = (p.gdes()->get_desx()-p.gipx())*(p.gdes()->get_desx()-p.gipx())+(p.gdes()->get_desy()-p.gipy())*(p.gdes()->get_desy()-p.gipy());
    double l2 = (p.gdes()->get_desx()-tinpox)*(p.gdes()->get_desx()-tinpox)+(p.gdes()->get_desy()-tinpoy)*(p.gdes()->get_desy()-tinpoy);
    if(l1-l2 > 1e-7 && tinpox <= max(p.gsou()->get_desx(),p.gdes()->get_desx()) && tinpox >= min(p.gsou()->get_desx(), p.gdes()->get_desx()) &&
       tinpoy <= max(p.gsou()->get_desy(),p.gdes()->get_desy()) && tinpoy >= min(p.gsou()->get_desy(), p.gdes()->get_desy()))
    {
        p.sinpo(tinpox, tinpoy);
        double slope = (p.gsou()->get_desy()-p.gdes()->get_desy())/(p.gsou()->get_desx()-p.gdes()->get_desx());
        double d1 = slope*(get_desx()-p.gsou()->get_desx())-(get_desy()-p.gsou()->get_desy());
        double d2 = slope*(n1->get_desx()-p.gsou()->get_desx())-(n1->get_desy()-p.gsou()->get_desy());
        double to;
        if(slope > 1e-7 && p.gsou()->get_desx()-p.gdes()->get_desx() > 1e-7 && p.gsou()->get_desy()-p.gdes()->get_desy() > 1e-7)
            to = -1.0;
        else if(slope < -1e-7 && p.gsou()->get_desx()-p.gdes()->get_desx() > 1e-7 && p.gdes()->get_desy()-p.gsou()->get_desy() > 1e-7)
            to = -1.0;
        else
            to = 1.0;
        if(abs(slope) < 1e-7 && get_desy()-p.gsou()->get_desy() > 0 && n1->get_desy()-p.gsou()->get_desy() < 0)
            return true;
        //若順序調換，會有bug(去你媽咪的精度問題)
        else if(slope*to > 0 && d1 < 0 && d2 > 0)
            return true;
        else if(slope*to < 0 && d1 > 0 && d2 < 0)
            return true;
        else if(fabs(p.gsou()->get_desx()-p.gdes()->get_desx()) < 1e-7 && get_desx()-p.gsou()->get_desx() < 0 && n1->get_desx()-p.gsou()->get_desx() > 0)
            return true;
        else
            return false;
    }
    else
        return true;
}
