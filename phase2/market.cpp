#include "market.h"
#include "map.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

struct Trade {
    int linetime;
    std::string brokerName;
    std::string action;
    std::string companyName;
    int price;
    int num;
    int expirationTime;
};

// Helper functions to get parent, left child, and right child indices
int parent(int i) { return (i - 1) / 2; }
int leftChild(int i) { return 2 * i + 1; }
int rightChild(int i) { return 2 * i + 2; }

// Heapify operation to maintain min heap property upward
void heapifyUpMin(std::vector<Trade>& heap, int index) {
    while (index > 0) {
        int parent_index = parent(index);
        if (heap[parent_index].price > heap[index].price ||
            (heap[parent_index].price == heap[index].price &&
             heap[parent_index].linetime > heap[index].linetime) ||
            (heap[parent_index].price == heap[index].price &&
             heap[parent_index].linetime == heap[index].linetime &&
             heap[parent_index].brokerName > heap[index].brokerName)) {
            std::swap(heap[parent_index], heap[index]);
            index = parent_index;
        } else {
            break;
        }
    }
}


// Heapify operation to maintain min heap property downward
void heapifyDownMin(std::vector<Trade>& heap, int index) {
    int left = leftChild(index);
    int right = rightChild(index);
    int smallest = index;

    if (left < heap.size() &&
        (heap[left].price < heap[index].price ||
         (heap[left].price == heap[index].price &&
          heap[left].linetime < heap[index].linetime) ||
         (heap[left].price == heap[index].price &&
          heap[left].linetime == heap[index].linetime &&
          heap[left].brokerName < heap[index].brokerName))) {
        smallest = left;
    }

    if (right < heap.size() &&
        (heap[right].price < heap[smallest].price ||
         (heap[right].price == heap[smallest].price &&
          heap[right].linetime < heap[smallest].linetime) ||
         (heap[right].price == heap[smallest].price &&
          heap[right].linetime == heap[smallest].linetime &&
          heap[right].brokerName < heap[smallest].brokerName))) {
        smallest = right;
    }

    if (smallest != index) {
        std::swap(heap[index], heap[smallest]);
        heapifyDownMin(heap, smallest);
    }
}


// Insert an element into the min heap
void insertMin(std::vector<Trade>& heap, const Trade& value) {
    heap.push_back(value);
    int index = heap.size() - 1;
    heapifyUpMin(heap, index);
}

// Remove the minimum element (root) from the min heap
void extractMin(std::vector<Trade>& heap) {
    if (heap.empty()) {
        // std::cout << "Heap is empty!" << std::endl;
        return;
    }

    heap[0] = heap.back();
    heap.pop_back();
    heapifyDownMin(heap, 0);
}

// Get the minimum element (root) of the min heap
Trade getMin(const std::vector<Trade>& heap) {
    if (!heap.empty()) {
        return heap[0];
    }
    // Returning a default Trade object indicating an empty heap
    return Trade{-1, "", "", "", -1, -1, -1};
}

// Display the elements of the heap
void display(const std::vector<Trade>& heap) {
    std::cout << "Heap: ";
    for (const auto& trade : heap) {
        std::cout << trade.price << " ";
    }
    std::cout << std::endl;
}

// // Functions for Max Heap
// void heapifyUpMax(std::vector<Trade>& heap, int index) {
//     while (index > 0 && heap[parent(index)].price < heap[index].price) {
//         std::swap(heap[parent(index)], heap[index]);
//         index = parent(index);
//     }
// }
void heapifyUpMax(std::vector<Trade>& heap, int index) {
    while (index > 0) {
        int parent_index = parent(index);
        if (heap[parent_index].price < heap[index].price ||
            (heap[parent_index].price == heap[index].price &&
             heap[parent_index].linetime >  heap[index].linetime) ||
            (heap[parent_index].price == heap[index].price &&
             heap[parent_index].linetime == heap[index].linetime &&
             heap[parent_index].brokerName > heap[index].brokerName)) {
            std::swap(heap[parent_index], heap[index]);
            index = parent_index;
        } else {
            break;
        }
    }
}


void heapifyDownMax(std::vector<Trade>& heap, int index) {
    int left = leftChild(index);
    int right = rightChild(index);
    int largest = index;

    if (left < heap.size() &&
        (heap[left].price > heap[index].price ||
         (heap[left].price == heap[index].price &&
          heap[left].linetime < heap[index].linetime) ||
         (heap[left].price == heap[index].price &&
          heap[left].linetime == heap[index].linetime &&
          heap[left].brokerName < heap[index].brokerName))) {
        largest = left;
    }

    if (right < heap.size() &&
        (heap[right].price > heap[largest].price ||
         (heap[right].price == heap[largest].price &&
          heap[right].linetime < heap[largest].linetime) ||
         (heap[right].price == heap[largest].price &&
          heap[right].linetime == heap[largest].linetime &&
          heap[right].brokerName < heap[largest].brokerName))) {
        largest = right;
    }

    if (largest != index) {
        std::swap(heap[index], heap[largest]);
        heapifyDownMax(heap, largest);
    }
}


void insertMax(std::vector<Trade>& heap, const Trade& value) {
    heap.push_back(value);
    int index = heap.size() - 1;
    heapifyUpMax(heap, index);
}

void extractMax(std::vector<Trade>& heap) {
    if (heap.empty()) {
        // std::cout << "Heap is empty!" << std::endl;
        return;
    }

    heap[0] = heap.back();
    heap.pop_back();
    heapifyDownMax(heap, 0);
}

Trade getMax(std::vector<Trade>& heap) {
    if (!heap.empty()) {
        return heap[0];
    }
    // Returning a default Trade object indicating an empty heap
    return Trade{-1, "", "", "", -1, -1, -1};
}
market::market(int argc, char** argv)
{	
}

void market::start()
{
	

    std::ifstream inputFile("output.txt");
    std::vector<std::vector<Trade>> sell;
    std::vector<std::vector<Trade>> buy;
    std::vector<std::string>company_names;
    std::vector<std::string>company1_names;//individual companies
    Map <std::string, pair<int,int>> broker_names;
    Map <std::string,int> broker_transfers;

    int total_money = 0;
    int completed_trades = 0;
    int shares_traded = 0;
    

        std::string line;
        while (std::getline(inputFile, line)) {
            if (line == "!@" || line == "@!")
                break;
            if(line == "TL"){continue;}
            std::istringstream iss(line);
            Trade newTrade;
            std::string broker, action,company1, priceStr, numStr;
            std::string company ="";
            int price, time, num, linetime, expirationTime;
            int index1 = -1;
            iss >> linetime;
            newTrade.linetime = linetime;
            iss >> broker;
            newTrade.brokerName = broker;
            iss >> action;
            newTrade.action = action;

            int separation_flag = true;
            // std::vector<std::string>companyline_names;
            std::vector<int>quantityline;
            std::vector<bool>no_mention;
            for(int i=0; i<company1_names.size();i++){
                // company_names.push_back(company1_names[i]);
                quantityline.push_back(0);
                no_mention.push_back(0);
            }
            while(iss>>company1 && company1[0]!='$'){
                bool sflag = false;

                if(company1.find_first_not_of("-0123456789") != std::string::npos){
                    if(!separation_flag){
                        quantityline[index1] = 1;
                        no_mention[index1]= 1;
                    }
                    for(int i =0; i<company_names.size();i++){
                    if(company1 == company1_names[i]){index1 = i;sflag = true; break;}
                    }
                    if(!sflag){
                        index1= company1_names.size();
                        company1_names.push_back(company1);
                        // companyline_names.push_back(company1);
                        quantityline.push_back(0);
                        no_mention.push_back(0);
                    }
                    
                    separation_flag = false;
                }
                else{

                    quantityline[index1] = std::stoi(company1);
                    separation_flag = true;

                }


            }
            if(!separation_flag){
                quantityline[index1] = 1;
            }
            // for(int i=0; i<company1_names.size();i++){
            //     std::cout<<quantityline[i];
            // }
            for(int i=0; i<company1_names.size();i++){
                if(quantityline[i]!=0){
                    company += company1_names[i];
                    company += " ";
                    if(no_mention[i]==1){
                    company += std::to_string(quantityline[i]);
                    company += " ";
                    }
                }
            }

            newTrade.companyName = company;
            
            
            company1.erase(0, 1);
            price = std::stoi(company1);

            iss  >> numStr >> time;
            numStr.erase(0, 1);
            num = std::stoi(numStr);
            if(time!=-1){
            expirationTime = time+linetime;
            }
            else{
                expirationTime = -1;
            }
            newTrade.expirationTime = expirationTime;
            newTrade.num = num;
            newTrade.price=price;
            int index = -1;
            bool add = 1;
            for(int i =0; i<company_names.size();i++){
                if(company == company_names[i]){index = i; break;}
            }
            if(index==-1){
            company_names.push_back(company);
            std::vector<Trade>c_sell;
            sell.push_back(c_sell);
            std::vector<Trade>c_buy;
            buy.push_back(c_buy);
            index = company_names.size()-1;
            }
            if(action=="SELL"){
                add = 0;
                if(buy[index].size()!=0){
                    int size = buy[index].size();
                    // std::cout<<size;
                    for(int i =0; i<size;i++){
                        if(getMax(buy[index]).expirationTime != -1 && (getMax(buy[index]).expirationTime < newTrade.linetime)){
                            extractMax(buy[index]);
                            // std::cout<<"yes";
                            add = 1;
                        }
                        else{
                            // std::cout<<getMax(buy[index]).price<<std::endl;
                            if(getMax(buy[index]).price > newTrade.price || getMax(buy[index]).price == newTrade.price){
                                // std::cout<<"yes"<<std::endl;
                                if(getMax(buy[index]).num > newTrade.num){
                                    // std::cout<<"yes1"<<std::endl;
                                    buy[index][0].num -= newTrade.num;
                                    total_money += buy[index][0].price*newTrade.num;
                                    completed_trades += 1;
                                    shares_traded += newTrade.num;
                                    Map <std::string, pair<int,int>> *foundElement = broker_names.find(buy[index][0].brokerName);
                                    if(foundElement != nullptr){
                                        foundElement->second.first += newTrade.num;
                                        broker_transfers[buy[index][0].brokerName]-=buy[index][0].price*newTrade.num;
                                    }
                                    else{
                                        broker_transfers.insert(buy[index][0].brokerName,-buy[index][0].price*newTrade.num);
                                        broker_names.insert(buy[index][0].brokerName,std::make_pair(newTrade.num,0));
                                    }
                                    Map <std::string, pair<int,int>> *foundElement0 = broker_names.find(newTrade.brokerName);
                                    if(foundElement0 != nullptr){
                                        foundElement0->second.second += newTrade.num;
                                        broker_transfers[newTrade.brokerName]+=buy[index][0].price*newTrade.num;
                                    }
                                    
                                    else{
                                        broker_transfers.insert(newTrade.brokerName,buy[index][0].price*newTrade.num);
                                        broker_names.insert(newTrade.brokerName,std::make_pair(0,newTrade.num));
                                    }

                                    // std::cout<<newTrade.brokerName<<" sold "<< newTrade.num <<" share of "<<buy[index][0].companyName<<" from "<<buy[index][0].brokerName<<" for $"<<buy[index][0].price<<"/share"<<std::endl;
                                    std::cout<<buy[index][0].brokerName<<" purchased "<< newTrade.num <<" share of "<<buy[index][0].companyName<<"from "<<newTrade.brokerName<<" for $"<<buy[index][0].price<<"/share"<<std::endl;
                                    add = 0;
                                    break;
                                }
                                else if(getMax(buy[index]).num < newTrade.num){
                                    // std::cout<<"yes2"<<std::endl;
                                    newTrade.num -= buy[index][0].num;

                                    total_money += buy[index][0].price*buy[index][0].num;
                                    completed_trades += 1;
                                    shares_traded += buy[index][0].num;

                                    Map <std::string, pair<int,int>> *foundElement1 = broker_names.find(buy[index][0].brokerName);
                                    if(foundElement1 != nullptr){
                                        foundElement1->second.first += buy[index][0].num;
                                        broker_transfers[buy[index][0].brokerName]-=buy[index][0].price*buy[index][0].num;
                                    }
                                    else{

                                        broker_names.insert(buy[index][0].brokerName,std::make_pair(buy[index][0].num,0));
                                        broker_transfers.insert(buy[index][0].brokerName,-buy[index][0].price*buy[index][0].num);
                                    }
                                    Map <std::string, pair<int,int>> *foundElement2= broker_names.find(newTrade.brokerName);
                                    if(foundElement2 != nullptr){
                                        foundElement2->second.second += buy[index][0].num;
                                        broker_transfers[newTrade.brokerName]+=buy[index][0].price*buy[index][0].num;
                                    }
                                    else{

                                        broker_names.insert(newTrade.brokerName,std::make_pair(0,buy[index][0].num));
                                        broker_transfers.insert(newTrade.brokerName,buy[index][0].price*buy[index][0].num);
                                    }

                                    
                                    // std::cout<<newTrade.brokerName<<" sold "<< newTrade.num <<" share of "<<buy[index][0].companyName<<" from "<<buy[index][0].brokerName<<" for $"<<buy[index][0].price<<"/share"<<std::endl;
                                    std::cout<<buy[index][0].brokerName<<" purchased "<< buy[index][0].num <<" share of "<<buy[index][0].companyName<<"from "<<newTrade.brokerName<<" for $"<<buy[index][0].price<<"/share"<<std::endl;
                                    extractMax(buy[index]);
                                    add = 1;
                                }
                                else{
                                    // std::cout<<"yes3"<<std::endl;
                                        
                                        add =0;

                                        total_money += buy[index][0].price*newTrade.num;
                                        completed_trades += 1;
                                        shares_traded += newTrade.num;
                                        Map <std::string, pair<int,int>> *foundElement3 = broker_names.find(buy[index][0].brokerName);
                                        if(foundElement3 != nullptr){
                                        foundElement3->second.first += newTrade.num;
                                        broker_transfers[buy[index][0].brokerName]-=buy[index][0].price*newTrade.num;
                                        }
                                        else{

                                            broker_names.insert(buy[index][0].brokerName,std::make_pair(newTrade.num,0));
                                            broker_transfers.insert(buy[index][0].brokerName,-buy[index][0].price*newTrade.num);
                                        }
                                        Map <std::string, pair<int,int>> *foundElement4 = broker_names.find(newTrade.brokerName);
                                        if(foundElement4 != nullptr){
                                            foundElement4->second.second += newTrade.num;
                                            broker_transfers[newTrade.brokerName]+=buy[index][0].price*newTrade.num;
                                        }
                                        else{

                                            broker_names.insert(newTrade.brokerName,std::make_pair(0,newTrade.num));
                                            broker_transfers.insert(newTrade.brokerName,buy[index][0].price*newTrade.num);
                                        }

                                        // std::cout<<newTrade.brokerName<<" sold "<< newTrade.num <<" share of "<<buy[index][0].companyName<<" from "<<buy[index][0].brokerName<<" for $"<<buy[index][0].price<<"/share"<<std::endl;
                                        std::cout<<buy[index][0].brokerName<<" purchased "<< newTrade.num <<" share of "<<buy[index][0].companyName<<"from "<<newTrade.brokerName<<" for $"<<buy[index][0].price<<"/share"<<std::endl;
                                        extractMax(buy[index]);
                                        break;
                                }
                            }
                            else{
                                insertMin(sell[index],newTrade);
                                add = 0;
                                break;
                            }
                        }
                    }
                    if(add){
                        insertMin(sell[index],newTrade);
                    }
                }
                else{
                    insertMin(sell[index],newTrade);
                    // std::cout<<newTrade.price;
                }
        }

            else if(action == "BUY"){
                add = false;
                if(sell[index].size()!=0){
                    int size = sell[index].size();
                    for(int i =0; i<size;i++){
                        // std::cout<<getMin(sell[index]).price;
                        if(getMin(sell[index]).expirationTime != -1 && (getMin(sell[index]).expirationTime < newTrade.linetime )){
                            extractMin(sell[index]);
                            // std::cout<<"yes";
                            add = 1;
                        }
                        else{
                            if(getMin(sell[index]).price < newTrade.price || getMin(sell[index]).price == newTrade.price){
                                if(getMin(sell[index]).num > newTrade.num){
                                    sell[index][0].num -= newTrade.num;
                                    total_money += sell[index][0].price*newTrade.num;
                                    completed_trades += 1;
                                    shares_traded += newTrade.num;


                                        Map <std::string, pair<int,int>> *foundElement5 = broker_names.find(newTrade.brokerName);
                                        if(foundElement5 != nullptr){
                                        foundElement5->second.first += newTrade.num;
                                        broker_transfers[newTrade.brokerName]-=sell[index][0].price*newTrade.num;
                                        }
                                        else{

                                            broker_names.insert(newTrade.brokerName,std::make_pair(newTrade.num,0));
                                            broker_transfers.insert(newTrade.brokerName,-sell[index][0].price*newTrade.num);
                                        }
                                        Map <std::string, pair<int,int>> *foundElement6 = broker_names.find(sell[index][0].brokerName);
                                        if(foundElement6 != nullptr){
                                            foundElement6->second.second += newTrade.num;
                                            broker_transfers[sell[index][0].brokerName]+=sell[index][0].price*newTrade.num;
                                            
                                        }
                                        else{

                                            broker_names.insert(sell[index][0].brokerName,std::make_pair(0,newTrade.num));
                                            broker_transfers.insert(sell[index][0].brokerName,sell[index][0].price*newTrade.num);
                                        }
                                    // std::cout<<newTrade.brokerName<<" sold "<< newTrade.num <<" share of "<<sell[index][0].companyName<<" from "<<sell[index][0].brokerName<<" for $"<<sell[index][0].price<<"/share"<<std::endl;
                                    std::cout<<newTrade.brokerName<<" purchased "<< newTrade.num <<" share of "<<sell[index][0].companyName<<"from "<<sell[index][0].brokerName<<" for $"<<sell[index][0].price<<"/share"<<std::endl;
                                    add = 0;
                                    break;
                                }
                                else if(getMin(sell[index]).num < newTrade.num){
                                    newTrade.num -= sell[index][0].num;

                                    total_money += sell[index][0].price*sell[index][0].num;
                                    completed_trades += 1;
                                    shares_traded += sell[index][0].num;


                                    Map <std::string, pair<int,int>> *foundElement7 = broker_names.find(newTrade.brokerName);
                                    if(foundElement7 != nullptr){
                                        foundElement7->second.first += sell[index][0].num;
                                        broker_transfers[newTrade.brokerName]-=sell[index][0].price*sell[index][0].num;
                                    }
                                    else{

                                        broker_names.insert(newTrade.brokerName,std::make_pair(sell[index][0].num,0));
                                        broker_transfers.insert(newTrade.brokerName,-sell[index][0].price*sell[index][0].num);
                                    }
                                    Map <std::string, pair<int,int>> *foundElement8 = broker_names.find(sell[index][0].brokerName);
                                    if(foundElement8 != nullptr){
                                        foundElement8->second.second += sell[index][0].num;
                                        broker_transfers[sell[index][0].brokerName]+=sell[index][0].price*sell[index][0].num;
                                    }
                                    else{

                                        broker_names.insert(sell[index][0].brokerName,std::make_pair(0,sell[index][0].num));
                                        broker_transfers.insert(sell[index][0].brokerName,sell[index][0].price*sell[index][0].num);
                                    }
                                    std::cout<<newTrade.brokerName<<" purchased "<< sell[index][0].num <<" share of "<<sell[index][0].companyName<<"from "<<sell[index][0].brokerName<<" for $"<<sell[index][0].price<<"/share"<<std::endl;
                                    // std::cout<<sell[index][0].brokerName<<" sold "<< newTrade.num <<" share of "<<sell[index][0].companyName<<" from "<<newTrade.brokerName<<" for $"<<buy[index][0].price<<"/share"<<std::endl;
                                    extractMin(sell[index]);
                                    add = 1;
                                }
                                else{
                                        
                                        add =0;
                                        total_money += sell[index][0].price*newTrade.num;
                                        completed_trades += 1;
                                        shares_traded += newTrade.num;

                                        Map <std::string, pair<int,int>> *foundElement1 = broker_names.find(newTrade.brokerName);
                                        if(foundElement1 != nullptr){
                                        foundElement1->second.first += newTrade.num;
                                        broker_transfers[newTrade.brokerName]-=sell[index][0].price*newTrade.num;
                                        }
                                        else{

                                            broker_names.insert(newTrade.brokerName,std::make_pair(newTrade.num,0));
                                            broker_transfers.insert(newTrade.brokerName,-sell[index][0].price*newTrade.num);
                                        }
                                        Map <std::string, pair<int,int>> *foundElement2 = broker_names.find(sell[index][0].brokerName);
                                        if(foundElement2 != nullptr){
                                            foundElement2->second.second += newTrade.num;
                                            broker_transfers[sell[index][0].brokerName]+=sell[index][0].price*newTrade.num;
                                        }
                                        else{

                                            broker_names.insert(sell[index][0].brokerName,std::make_pair(0,newTrade.num));
                                            broker_transfers.insert(sell[index][0].brokerName,sell[index][0].price*newTrade.num);
                                        }
                                        // std::cout<<newTrade.brokerName<<" sold "<< newTrade.num <<" share of "<<sell[index][0].companyName<<" from "<<sell[index][0].brokerName<<" for $"<<sell[index][0].price<<"/share"<<std::endl;
                                        std::cout<<newTrade.brokerName<<" purchased "<< newTrade.num <<" share of "<<sell[index][0].companyName<<"from "<<sell[index][0].brokerName<<" for $"<<sell[index][0].price<<"/share"<<std::endl;
                                        extractMin(sell[index]);
                                        break;
                                }
                            }
                            else{
                                insertMax(buy[index],newTrade);
                                add = 0;
                                break;
                            }
                        }
                    }
                    if(add){
                        insertMax(buy[index],newTrade);
                    }
                }
                else{
                    insertMax(buy[index],newTrade);
                }

            }

            // Table.push_back(newTrade);
        }


        std::cout<<std::endl<<"---End of Day---"<<std::endl;
        std::cout<<"Total Amount of Money Transferred: $"<<total_money<<std::endl;
        std::cout<<"Number of Completed Trades: "<<completed_trades<<std::endl;
        std::cout<<"Number of Shares Traded: "<<shares_traded<<std::endl;
        for (Map<std::string, std::pair<int, int>>::Iterator it = broker_names.begin(); it != broker_names.end(); ++it) {
        std::cout << (*it).first << " bought " << (*it).second.first << " and sold " << (*it).second.second << " for a net transfer of $"<<broker_transfers[(*it).first] << std::endl;
        }
}
