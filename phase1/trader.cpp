#include "receiver.h"
#include <vector>

struct Trade {
    std::vector<int> quantities;
    int price;
    char action; // Assuming 's' for sell and 'b' for buy
};

void arbitrage (std::vector<Trade>& Table, int index, std::vector<int>& subset, int& profit, std::vector<int>& final_subset ){

    
    if(index!=0){
    arbitrage (Table, index-1, subset,profit, final_subset);
    }
    subset.push_back(index);
    int profit1 = 0;
    bool flag = true;
        for(int j = 0; j<Table[0].quantities.size();j++){
            int sum = 0;
            for(int i =0; i<subset.size();i++){
                if(Table[subset[i]].action == 's'){
                    sum -= Table[subset[i]].quantities[j];
                }
                else{
                sum += Table[subset[i]].quantities[j];
                }
            }
            if (sum !=0){flag = false; break;}
        }
    if(flag){
        for(int i =0; i<subset.size();i++){
            if(Table[subset[i]].action == 's'){
                profit1 -= Table[subset[i]].price;
            }
            else{
                profit1 += Table[subset[i]].price;
            }
        }
        if(profit1>profit){
            profit = profit1;
            final_subset.clear();
            final_subset.insert(final_subset.begin(), subset.begin(), subset.end());
        }
        
    }
    if(index!=0){
    arbitrage (Table, index-1, subset,profit, final_subset);
    }
    subset.pop_back();
    return;
}


struct Trade1 {
    std::vector<int> quantities;
    int price;
    char action; // Assuming 's' for sell and 'b' for buy
    int numorders;
};

void arbitrage1 (std::vector<Trade1>& Table, int index, std::vector<int>& subset,std::vector<int>& nums, int& profit, std::vector<int>& final_subset, std::vector<int>& final_nums ){

    
    if(index!=0){
    arbitrage1 (Table, index-1, subset,nums,profit, final_subset,final_nums);
    }
    subset.push_back(index);
    int quantity = Table[index].numorders;
    for(int i =quantity;i>0;i--){
    nums.push_back(i);
    int profit1 = 0;
    bool flag = true;
        for(int j = 0; j<Table[0].quantities.size();j++){
            int sum = 0;
            for(int i =0; i<subset.size();i++){
                if(Table[subset[i]].action == 's'){
                    sum -= (Table[subset[i]].quantities[j])*nums[i];
                }
                else{
                sum += (Table[subset[i]].quantities[j])*nums[i];
                }
            }
            if (sum !=0){flag = false; break;}
        }
    if(flag){
        // std::cout<<"yes";
        for(int i =0; i<subset.size();i++){
            if(Table[subset[i]].action == 's'){
                profit1 -= Table[subset[i]].price*nums[i];
            }
            else{
                profit1 += Table[subset[i]].price*nums[i];
            }
        }
        if(profit1>profit){
            // std::cout<<"yes";
            profit = profit1;
            final_subset.clear();
            final_subset.insert(final_subset.begin(), subset.begin(), subset.end());
            final_nums.clear();
            final_nums.insert(final_nums.begin(), nums.begin(), nums.end());
        }
        
    }
    if(index!=0){
    arbitrage1 (Table, index-1, subset,nums,profit, final_subset,final_nums);
    }
    nums.pop_back();
    }
    subset.pop_back();
    return;
}

int main(int argc, char *argv[]) {
    // // Check if there are enough command-line arguments
    // if (argc < 2) {
    //     std::cerr << "Usage: " << argv[0] << " <arg1> <arg2> <arg3>" << std::endl;
    //     return 1; // Return an error code
    // }

    // Convert command-line arguments to integers
    int arg1 = std::atoi(argv[1]);

    // Use conditional statements to execute different code based on arguments
    if (arg1 == 1) {
        // Code for argument 1
        bool message_flag = true;
        Receiver rcv;
        // sleep(1);
        std::string message;
        while (message_flag == true){
        message += rcv.readIML();
        if(message[message.size()-1]== '$'){message_flag=false;}
        }
        std::istringstream iss(message);
        std::string line;
        std::string output = "";

        std::vector<std::string> company_names;
        std::vector<int> price_breaks;
        bool isFirstline = true;
        std::vector<int> counts;
        std::vector<std::vector<int>> no_trade_s;
        std::vector<std::vector<int>> no_trade_b;
        int company_index = 0;

        while(std::getline(iss, line, char(13))){
            if(isFirstline){isFirstline = false;}
            std::istringstream linestream(line);
            std::string company_name;
            int price;
            std::string action;
            int index;
            bool flag = false;
            linestream >> company_name >> price >> action;
            for(int i = 0; i<company_names.size(); i++){
                if(company_names[i] == company_name){flag = true; index = i; counts[index]++;break;}
            }
            if( !flag){
                company_names.push_back(company_name);
                price_breaks.push_back(price);
                counts.push_back(0);
                std::vector<int> notrade_s;
                std::vector<int> notrade_b;
                no_trade_b.push_back(notrade_b);
                no_trade_s.push_back(notrade_s);
                index = company_index;
                company_index ++;
            }
            // output printing starts here

            std::string output = "";
            if(counts[index] == 0) {
                if(action[0] == 's'){
                    output += company_name + " " + std::to_string(price)+ " " + "b";
                    
                    
                }
                else{
                    output += company_name + " " + std::to_string(price)+ " " + "s";
                    
                }
            }
            else  {

                    if(price > price_breaks[index] && action[0] == 'b'){
                        

                        if(no_trade_b[index].size()!=0 && no_trade_b[index][0]!=price){
                            no_trade_b[index].pop_back();
                        }
                        if(no_trade_b[index].size()!=0 && no_trade_b[index][0]==price){
                            output += "No Trade";
                        }
                        else if(no_trade_s[index].size()!=0 && no_trade_s[index][0] == price){output += "No Trade";no_trade_s[index].pop_back();}
                        else{
                        price_breaks[index] = price;
                        output += company_name + " " + std::to_string(price)+ " " + "s";
                        }
                    }
                    else if (price < price_breaks[index] && action[0] == 's'){
                        if(no_trade_s[index].size()!=0 && no_trade_s[index][0]!=price){
                            no_trade_s[index].pop_back();
                        }
                        if(no_trade_s[index].size()!=0 && no_trade_s[index][0]==price){
                            output += "No Trade";
                        }
                        else if(no_trade_b[index].size()!=0 && no_trade_b[index][0] == price){output += "No Trade";no_trade_b[index].pop_back();}
                        else{
                        price_breaks[index] = price;
                        output += company_name + " " + std::to_string(price)+ " " + "b";
                        }
                    }
                    else {

                        if(action[0] == 's'){
                            if(no_trade_s[index].size() == 1){

                                if(no_trade_s[index][0] > price){

                                    no_trade_s[index].pop_back();
                                    no_trade_s[index].push_back(price);

                                }
                            }
                            else{
                                no_trade_s[index].push_back(price);
                            }
                        }
                        else{

                            if(no_trade_b[index].size() == 1){

                                if(no_trade_b[index][0] < price){

                                    no_trade_b[index].pop_back();
                                    no_trade_b[index].push_back(price);

                                }
                            }
                            else{
                                no_trade_b[index].push_back(price);
                            }
                        }

                        output += "No Trade";
                        
                    }

            }
            std::cout << output << std::endl;
        }
        // std::cout << "Executing code for argument 1" << std::endl;
    } else if (arg1 == 2) {
        // Code for argument 2

        
            bool message_flag = true;
            Receiver rcv;
            // sleep(1);
            std::string message;

            while (message_flag == true){
            message += rcv.readIML();
            if(message[message.size()-1]== '$'){message_flag=false;}
            }
            // sleep(5);

            std::istringstream iss(message);
            std::string line;
            std::string output = "";

            std::vector<std::string> Company_names;
            int index = 0;
            std::vector<Trade> Table;
            std::vector<std::string> Output_line;
            int final_profit = 0;
            

            while(std::getline(iss, line, char(13))){
                std::string output = "";
                Trade Order;
                int stream = 0;
                std::istringstream linestream(line);
                std::string company_name;
                int price;
                int quantity;

                for(int k = 0; k < Company_names.size(); k++){
                            Order.quantities.push_back(0);
                }
                std::string action;
                while (linestream >> company_name >> quantity && company_name.find_first_not_of("-0123456789") != std::string::npos) {
                    output += company_name;
                    output += " ";
                    output += std::to_string(quantity);
                    output += " ";
                    // std::cout<<company_name<<quantity<<std::endl;
                    bool flag = false;
                    for(int i = 0; i<Company_names.size(); i++){
                    if(Company_names[i] == company_name){flag = true; index = i;break;}
                    }
                    if(!flag){
                        
                        Company_names.push_back(company_name);
                        for(int j = 0; j<Table.size();j++){
                            Table[j].quantities.push_back(0);
                        }
                        index++;
                        Order.quantities.push_back(quantity);
                    }
                    else {
                        Order.quantities[index] = quantity;
                    }
                    stream++;
                }
                std::istringstream linestream1(line);
                for(int i = 0; i<stream;i++){
                    linestream1 >> company_name >> quantity;
                }
                // std::cout<<linestream.str()<<std::endl;
                linestream1 >> price >> action;

                output += std::to_string(price);
                output += " ";
                if(action[0]=='b'){
                    output += "s";
                }
                else{
                    output += "b";
                }

                Order.action = action[0];
                Order.price = price;
                int cindex = -1;
                bool add = 1;
                for(int i = 0; i<Table.size();i++){
                    bool cflag = true;
                    for( int j = 0; j<Company_names.size(); j++){
                        if(Table[i].quantities[j] != Order.quantities[j]){cflag = false; break;}
                    }
                    if(cflag == true){cindex = i;break;}
                }
                if(cindex != -1){
                    if(Table[cindex].action == 'b' && Order.action == 'b'){
                        if(Table[cindex].price < Order.price){
                            Table.erase(Table.begin()+cindex);
                            Output_line.erase(Output_line.begin()+cindex);
                            add = 1;
                        }
                        else{
                            add = 0;
                            std::cout<<"No Trade"<<std::endl;
                        }
                    }
                    else if(Table[cindex].action == 's' && Order.action == 's'){
                        if(Table[cindex].price > Order.price){
                            Table.erase(Table.begin()+cindex);
                            Output_line.erase(Output_line.begin()+cindex);
                            add = 1;
                        }
                        else{
                            add = 0;
                            std::cout<<"No Trade"<<std::endl;
                        }
                    }
                    else if(Table[cindex].action == 'b' && Order.action == 's' && Table[cindex].price == Order.price){
                        Table.erase(Table.begin()+cindex);
                        Output_line.erase(Output_line.begin()+cindex);
                        add = 0;
                        std::cout<<"No Trade"<<std::endl;
                    }
                    else if(Table[cindex].action == 's' && Order.action == 'b' && Table[cindex].price == Order.price){
                        Table.erase(Table.begin()+cindex);
                        Output_line.erase(Output_line.begin()+cindex);
                        add = 0;
                        std::cout<<"No Trade"<<std::endl;
                    }
                
                }
                if(add){

                Table.push_back(Order);
                Output_line.push_back(output);

                int table_index = Table.size() - 1 ;
                if(table_index==0){std::cout<<"No Trade"<<std::endl;}
                if(table_index>0){
                std::vector<int> subset;
                std::vector<int> final_subset;
                int profit = 0;
                subset.push_back(table_index);
                arbitrage(Table,table_index-1,subset,profit,final_subset);
                if(final_subset.size()!=0){
                    for (int i=0; i<final_subset.size();i++) {
                    std::cout<<Output_line[final_subset[i]]<<"#"<<std::endl;
                    Table.erase(Table.begin()+final_subset[i]);
                    Output_line.erase(Output_line.begin()+final_subset[i]);
                    }
                    // std::cout<<profit<<std::endl;
                    final_profit += profit;
                }
                else{
                    std::cout<<"No Trade"<<std::endl;
                }
                }
                }
            }
            // for (const Trade& trade : Table) {
            //     int index = 0;
            //     for (int quantity : trade.quantities) {
            //         std::cout <<"company:"<< Company_names[index] << " ";
            //         std::cout <<"quantity:"<< quantity << " ";
            //         index ++;
            //     }
            //     std::cout << std::endl;

            //     std::cout << "Price: " << trade.price << std::endl;
            //     std::cout << "Action: " << trade.action << std::endl;
            // }
            std::cout<<final_profit<<std::endl;


        // std::cout << "Executing code for argument 2" << std::endl;
    } else if (arg1 == 3) {
        // Code for argument 3

        bool message_flag = true;
        Receiver rcv;
        // sleep(1);
        std::string message;

        while (message_flag == true){
        message += rcv.readIML();
        if(message[message.size()-1]== '$'){message_flag=false;}
        }
        // sleep(5);
        std::istringstream iss(message);
        std::string line;
        std::string output = "";
        std::vector<std::string> Company_names;
        int index = 0;
        std::vector<Trade1> Table;
        std::vector<std::string> Output_line;
        int final_profit = 0;
        

        while(std::getline(iss, line, char(13))){
            std::string output = "";
            Trade1 Order;
            int stream = 0;
            std::istringstream linestream(line);
            std::string company_name;
            int price;
            int quantity;
            int numorders;

            for(int k = 0; k < Company_names.size(); k++){
                        Order.quantities.push_back(0);
            }
            std::string action;
            while (linestream >> company_name >> quantity && company_name.find_first_not_of("-0123456789") != std::string::npos) {
                output += company_name;
                output += " ";
                output += std::to_string(quantity);
                output += " ";
                // std::cout<<company_name<<quantity<<std::endl;
                bool flag = false;
                for(int i = 0; i<Company_names.size(); i++){
                if(Company_names[i] == company_name){flag = true; index = i;break;}
                }
                if(!flag){
                    
                    Company_names.push_back(company_name);
                    for(int j = 0; j<Table.size();j++){
                        Table[j].quantities.push_back(0);
                    }
                    index++;
                    Order.quantities.push_back(quantity);
                }
                else {
                    Order.quantities[index] = quantity;
                }
                stream++;
            }
            // std::cout<<stream<<std::endl;
            std::istringstream linestream1(line);
            for(int i = 0; i<stream;i++){
                linestream1 >> company_name >> quantity;
            }
            // std::cout<<linestream.str()<<std::endl;
            linestream1 >> price >> numorders >> action;
            output += std::to_string(price);
            Order.action = action[0];
            Order.price = price;
            Order.numorders = numorders;
            // std::cout<<numorders<<std::endl;
            int cindex = -1;
            bool add = 1;
            int minimum = 0;
            for(int i = 0; i<Table.size();i++){
                bool cflag = true;
                for( int j = 0; j<Company_names.size(); j++){
                    if(Table[i].quantities[j] != Order.quantities[j]){cflag = false; break;}
                }
                if(cflag == true){cindex=i;break;}
            }
            if(cindex!=-1){
                // std::cout<<cindex<<std::endl;
                if(Table[cindex].action == 'b' && Order.action == 'b' && Table[cindex].price == Order.price){
                    Order.numorders  += Table[cindex].numorders ;
                    // std::cout<<Order.numorders<<std::endl;
                    add = 1;
                    Table.erase(Table.begin()+cindex);
                    // std::cout<<Table.size()<<std::endl;
                    Output_line.erase(Output_line.begin()+cindex);
                }
                else if(Table[cindex].action == 's' && Order.action == 's' &&  Table[cindex].price == Order.price){
                    Order.numorders  += Table[cindex].numorders ;
                    add = 1;
                    Table.erase(Table.begin()+cindex);
                    Output_line.erase(Output_line.begin()+cindex);

                }
                else if(Table[cindex].action == 'b' && Order.action == 's' && Table[cindex].price == Order.price){
                    if(Table[cindex].numorders > Order.numorders){
                        Table[cindex].numorders -= Order.numorders;
                        add = 0;
                        std::cout<<"No Trade"<<std::endl;
                    }
                    else if(Table[cindex].numorders < Order.numorders){
                        add = 1;
                        Order.numorders -=  Table[cindex].numorders;
                        Table.erase(Table.begin()+cindex);
                        Output_line.erase(Output_line.begin()+cindex);
                    }
                    else if(Table[cindex].numorders == Order.numorders){
                        Table.erase(Table.begin()+cindex);
                        Output_line.erase(Output_line.begin()+cindex);
                        add = 0;
                        std::cout<<"No Trade"<<std::endl;
                    }
                    // Table.erase(Table.begin()+cindex);
                    // Output_line.erase(Output_line.begin()+cindex);
                    // add = 0;
                    // std::cout<<"No Trade"<<std::endl;
                }
                else if(Table[cindex].action == 's' && Order.action == 'b' && Table[cindex].price == Order.price){
                    if(Table[cindex].numorders > Order.numorders){
                        Table[cindex].numorders -= Order.numorders;
                        add = 0;
                        std::cout<<"No Trade"<<std::endl;
                    }
                    else if(Table[cindex].numorders < Order.numorders){
                        add = 1;
                        Order.numorders -=  Table[cindex].numorders;
                        Table.erase(Table.begin()+cindex);
                        Output_line.erase(Output_line.begin()+cindex);
                    }
                    else if(Table[cindex].numorders == Order.numorders){

                        Table.erase(Table.begin()+cindex);
                        Output_line.erase(Output_line.begin()+cindex);
                        add = 0;
                        std::cout<<"No Trade"<<std::endl;
                    }
                }
            
            }
            if(add){
            
            
            Table.push_back(Order);
            Output_line.push_back(output);

            int table_index = Table.size() - 1 ;
            if(table_index==0){std::cout<<"No Trade"<<std::endl;}

            if(table_index > 0){
            std::vector<int> subset;
            std::vector<int> final_subset;
            
            std::vector<int> nums;
            std::vector<int> final_nums;
            int profit = 0;
            subset.push_back(table_index);

            for(int l=Table[table_index].numorders; l>0;l--){
                nums.push_back(l);
                arbitrage1(Table,table_index-1,subset,nums,profit,final_subset,final_nums);
                nums.pop_back();
                
            }

            // arbitrage(Table,table_index-1,subset,profit,final_subset);
            
            // std::cout<<profit<<std::endl;
            if(final_subset.size()==0){
                std::cout<<"No Trade"<<std::endl;
            }
            if(final_subset.size()!=0){
                
                // for (int i=0; i<final_subset.size();i++){
                //     if(Table[final_subset[i]].numorders < Table[final_subset[minimum]].numorders){minimum = i;}
                    
                // }
                for (int i=0; i<final_subset.size();i++) {
                if(Table[final_subset[i]].action=='b'){
                    output = "s";
                }
                else{
                    output = "b";
                }
                std::cout<<Output_line[final_subset[i]]<<" "<<final_nums[i]<<" "<<output<<std::endl;
                Table[final_subset[i]].numorders -= final_nums[i];
                if(Table[final_subset[i]].numorders == 0){
                Table.erase(Table.begin()+final_subset[i]);
                Output_line.erase(Output_line.begin()+final_subset[i]);
                }
                }
                
                final_profit += profit;
            }
            
            }
            }
        }
        // for (const Trade& trade : Table) {
        //     int index = 0;
        //     for (int quantity : trade.quantities) {
        //         std::cout <<"company:"<< Company_names[index] << " ";
        //         std::cout <<"quantity:"<< quantity << " ";
        //         index ++;
        //     }
        //     std::cout << std::endl;

        //     std::cout << "Price: " << trade.price << std::endl;
        //     std::cout << "Action: " << trade.action << std::endl;
        //     std::cout << "Action: " << trade.numorders << std::endl;
        // }
        std::cout<<final_profit<<std::endl;


        // std::cout << "Executing code for argument 3" << std::endl;
    } else{
    }

    // You can also use arg2 and arg3 in your code as needed

    return 0; // Return success code
}