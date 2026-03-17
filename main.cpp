//__________HEADER FILES AND DEFINITIONS FOR THE ABBREVIATION__________

#include <SFML/Graphics.hpp>
#include <bits/stdc++.h>
#include <conio.h>

#define ll long long
#define ld long double
#define pb push_back
#define mp make_pair

using namespace std;

//__________MAIN MAP ARRAYS__________

vector <pair<char, pair<ld, ld>>> Coordinates;
vector<pair<string, ld>> tall;
vector<string> method;

//__________MAIN WORK STAGES__________

void part_1();
vector<string> Big_M(vector<pair<string, ld>>, ll, ll, char, char);
void part_2();

//__________TRANSFORMING THE MAP INTO A MATHEMATICAL PROGRAMMING PROBLEM USING THE BIG_M METHOD__________

void part_1(){

    ll num_pnt;
    cout << "Enter number of points : ";
    cin >> num_pnt;
    cout << endl;

    for (int i = 0; i < num_pnt; i++){
        char ch;
        ld x, y;
        cout << "Enter name point_" << i + 1 << " : ";
        cin >> ch;
        cout << "Enter (x, y) point_" << i + 1 << " : ";
        cin >> x >> y;
        Coordinates.pb(mp(ch ,mp(x, y)));
    }

    cout << endl;
    char begin, end;
    cout << "Enret name begin point : ";
    cin >> begin;
    cout << "Enret name end point : ";
    cin >> end;
    cout << endl;

    ll num_lines;
    cout << "Enter number of lines : ";
    cin >> num_lines; 
    cout << endl;

    for (int i = 0; i < num_lines; i++){
        char ch1, ch2;
        ld x1, x2, y1, y2;
        cout << "Enter name line_" << i + 1 << " : ";
        cin >> ch1 >> ch2;
        for (int i = 0; i < num_pnt; i++){
            if (Coordinates[i].first == ch1){
                x1 = Coordinates[i].second.first;
                y1 = Coordinates[i].second.second;
            }
            if (Coordinates[i].first == ch2){
                x2 = Coordinates[i].second.first;
                y2 = Coordinates[i].second.second;
            }
        }
        tall.pb(mp(string(1, ch1) + string(1, ch2), sqrt(pow(fabs(x1 - x2), 2) + pow(fabs(y1 - y2), 2))));
    }cout << endl;

    ll  cnt = 2;
    for (int i = 0; i < num_lines; i++){
        if (tall[i].first[1] != end){
            cnt++;
        }
    }

    cout << endl << "\tM = 10^5" << endl;

    vector<string> method = Big_M(tall, cnt, num_lines, begin, end);

}

//__________SOLVE THE PROBLEM USING THE BIG_M METHOD AND SHOW THE SIMPLEX TABLES AND THE SOLUTION__________

vector<string> Big_M(vector<pair<string, ld>> tall, ll cnt, ll num_lines, char begin, char end){

    ll m = 1e5;
    ld ans = 2 * m;
    ld Simblex[cnt + 1][num_lines + cnt + 1];

    for (int i = 0; i < cnt + 1; i++){
        for (int j = 0; j < num_lines + cnt + 1; j++){
            Simblex[i][j] = 0;
        }
    }

    Simblex[0][num_lines + cnt] = 2 * m;
    Simblex[1][num_lines + cnt] = 1;
    Simblex[2][num_lines + cnt] = 1;

    int t = num_lines;
    for (int i = 1; i < cnt + 1; i++){
        Simblex[i][t] = 1;
        t++;
    }

    for (int i = 0; i < cnt + 1; i++){
        for (int j = 0; j < num_lines; j++){
            if (i == 0){
                if (tall[j].first[0] == begin){
                    Simblex[i][j] = 2 * m - tall[j].second;
                }else{
                    Simblex[i][j] = -tall[j].second;
                }
            }else{
                if (i == 1){
                    if (tall[j].first[0] == begin){
                        Simblex[i][j] = 1;
                    }
                }else{
                    if (i == 2){
                        if (tall[j].first[1] == end){
                            Simblex[i][j] = 1;
                        }
                    }
                }
            }
        }
    }

    int it = 3;
    for (int i = 0; i < num_lines; i++){
        for (int j = i + 1; j < num_lines; j++){
            if (tall[i].first[1] == tall[j].first[0]){
                Simblex[it][i] = 1;
                Simblex[it][j] = -1;
                it++;
            }
        }
    }

    bool choise = 0;
    for (int i = 0; i < num_lines; i++){
        if (Simblex[0][i] > 0) {choise = 1; break;}
    }

    string s_1[cnt + 1], s_2[num_lines + cnt + 1];
    s_1[0] = "Z";
    s_2[num_lines + cnt] ="RHS";
    for (int i = 0; i < num_lines; i++){
        s_2[i] = "X" + tall[i].first;
    }
    for(int i = 1; i < cnt + 1; i++){
        s_1[i] = "R" + to_string(i);
        s_2[i + num_lines - 1] = "R" + to_string(i);
    }

    for (int i = 0; i < num_lines + cnt + 3; i++) cout << "__________";
    cout << endl << endl;
    cout << setw(11) << "|";
    for (int i = 0; i < num_lines + cnt; i++){
        cout << setw(10) << s_2[i];
    }
    cout << "|" << setw(10) << s_2[num_lines + cnt];
    cout << endl;
    for (int i = 0; i < num_lines + cnt + 3; i++) cout << "----------";

    for (int i = 0; i < cnt + 1; i++){
        cout << endl << setw(10) << s_1[i] << "|";
        for (int j = 0; j < num_lines + cnt + 1; j++){
            if (j == num_lines + cnt) cout << "|";
            cout << setw(10) << Simblex[i][j];
        }
    }cout << endl;

    while (choise == 1){

        ld in_e = 0;
        for (int i = 0; i < num_lines; i++){
            in_e = max(in_e, Simblex[0][i]);
        }

        int index_in;
        for (int i = 0; i < num_lines; i++){
            if (Simblex[0][i] == in_e){
                index_in = i;
                break;
            }
        }

        ld out_e = 1e5;
        for (int i = 1; i < cnt + 1; i++){
            if (Simblex[i][index_in] <= 0) continue;
            out_e = min(out_e, Simblex[i][num_lines + cnt] / Simblex[i][index_in]);
        }

        int index_out;
        for (int i = 1; i < cnt + 1; i++){
            if (Simblex[i][index_in] <= 0) continue;
            else{
                if (Simblex[i][num_lines + cnt] / Simblex[i][index_in] == out_e){
                    index_out = i;
                    break;
                }
            }
        }

        s_1[index_out] = s_2[index_in];

        ld arr_in[cnt + 1];
        for (int i = 0; i < cnt + 1; i++){
            arr_in[i] = Simblex[i][index_in];
        }

        ld c_e = Simblex[index_out][index_in];

        for (int i = 0; i < num_lines + cnt + 1; i++){
            Simblex[index_out][i] /= c_e;
        }

        for (int i = 0; i < cnt + 1; i++){
            if (i == index_out) continue;
            for (int j = 0; j < num_lines + cnt + 1; j++){
                Simblex[i][j] = Simblex[i][j] - arr_in[i] * Simblex[index_out][j];
            }
        }

        ans = Simblex[0][num_lines + cnt];

        for (int i = 0; i < num_lines + cnt + 3; i++) cout << "__________";
        cout << endl << endl;
        cout << setw(11) << "  |";
        for (int i = 0; i < num_lines + cnt; i++){
            cout << setw(10) << s_2[i];
        }
        cout << "|" << setw(10) << s_2[num_lines + cnt];
        cout << endl;
        for (int i = 0; i < num_lines + cnt + 3; i++) cout << "----------";

        for (int i = 0; i < cnt + 1; i++){
            cout << endl << setw(10) << s_1[i] << "|";
            for (int j = 0; j < num_lines + cnt + 1; j++){
                if (j == num_lines + cnt) cout << "|";
                cout << setw(10) << fixed << setprecision(3) << Simblex[i][j];
            }
        }cout << endl;

        choise = 0;
        for (int i = 0; i < num_lines; i++){
            if (Simblex[0][i] > 0) {choise = 1; break;}
        }

    }

    
    for (int i = 1; i < cnt; i++){
        if (Simblex[i][num_lines + cnt] == 1){
            method.pb(s_1[i]);
        }
    }

    cout << endl << "\tZ = " << ans << endl;
    return method;
    
}

//__________THE MAP WAS DRAWN AND THE MOST SUITABLE ROUTE WAS COLORED GREEN__________

void part_2(){
    if (Coordinates.empty()) return;

    sf::RenderWindow window(sf::VideoMode(sf::Vector2u(800, 600)), "Graph");

    ld minX = Coordinates[0].second.first, maxX = Coordinates[0].second.first;
    ld minY = Coordinates[0].second.second, maxY = Coordinates[0].second.second;
    for (auto &p : Coordinates){
        if (p.second.first < minX) minX = p.second.first;
        if (p.second.first > maxX) maxX = p.second.first;
        if (p.second.second < minY) minY = p.second.second;
        if (p.second.second > maxY) maxY = p.second.second;
    }

    float margin = 50.f;
    float scaleX = (800.f - 2*margin) / (maxX - minX);
    float scaleY = (600.f - 2*margin) / (maxY - minY);

    auto getScaledPoint = [&](ld x, ld y){
        float px = margin + (float)((x - minX) * scaleX);
        float py = 600.f - (margin + (float)((y - minY) * scaleY));
        return sf::Vector2f(px, py);
    };

    auto isRed = [&](const string& lineName){
        return find(method.begin(), method.end(), "X" + lineName) != method.end();
    };

    sf::Font font;
    if (!font.openFromFile("C:/Windows/Fonts/arial.ttf")) {
        cout << "Font not found!" << endl;
    }

    while (window.isOpen()){
        while (auto event = window.pollEvent()){
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear(sf::Color::Black);

        for (auto &l : tall){
            char start = l.first[0];
            char end = l.first[1];

            sf::Vector2f A, B;
            for (auto &p : Coordinates){
                if (p.first == start) A = getScaledPoint(p.second.first, p.second.second);
                if (p.first == end)   B = getScaledPoint(p.second.first, p.second.second);
            }

            sf::Color color = isRed(l.first) ? sf::Color::Green : sf::Color::White;

            sf::Vertex line[2];
            line[0].position = A; line[0].color = color;
            line[1].position = B; line[1].color = color;

            window.draw(line, 2, sf::PrimitiveType::Lines);
        }

        float radius = 4.f;
        for (auto &p : Coordinates){
            sf::Vector2f pos = getScaledPoint(p.second.first, p.second.second);

            sf::CircleShape point(radius);
            point.setFillColor(sf::Color::Green);
            point.setPosition(pos - sf::Vector2f(radius, radius));
            window.draw(point);

            if (!font.getInfo().family.empty()){
                sf::Text text(font, sf::String(p.first), 14);
                text.setFillColor(sf::Color::White);
                sf::Vector2f offset(8.f, -18.f);

                for (auto &l : tall) {
                    if (l.first[0] == p.first || l.first[1] == p.first) {
                    char other = (l.first[0] == p.first ? l.first[1] : l.first[0]);
                    for (auto &q : Coordinates) {
                        if (q.first == other) {
                            if (q.second.second > p.second.second)
                            offset.y = 8.f;
                            break;
                        }
                    }
                    break;
                    }
                }

                text.setPosition(pos + offset);

                window.draw(text);
            }
        }

        window.display();
    }
}

//__________ALL FUNCTIONS ARE COMBINED INTO THE MAIN EXECUTION FUNCTION__________

int main(){

    part_1();

    sort (method.begin(), method.end());

    cout << "\t";
    for (int i = 0; i < method.size(); i++){
        cout << method[i][1] << method[i][2];
        if (i != method.size() - 1) cout << " --> ";
    }cout << endl;
    
    part_2();
    getch();

}