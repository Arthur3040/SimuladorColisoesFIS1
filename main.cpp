#include <SFML/Window.hpp>     
#include <iostream>
#include <SFML/Graphics.hpp>    
#include <cmath>
#include <cstdlib>
//quantidade de bolas, basta mudar o numero para ter mais ou menos bolas
#define N 10
int i,j;
float dx,dy, distance, 
      centerix,centeriy,centerjx,centerjy, 
      vn1,vn2,vt1,vt2,
      unx,uny,utx,uty,
      vnf1,vnf2,
      r1,r2,over,
      e=0.8;

int main()
{
//mudar tamanho da tela basta alterar os valores do VideoMode
sf::RenderWindow window(sf::VideoMode(1200, 800), "Simulador", sf::Style::Default);                         
sf::Vector2u window_size = window.getSize();
sf::Event events;
sf::FloatRect boulder[N];                   
window.setFramerateLimit(60);

sf::CircleShape bolas[N];
sf::Vector2f velocidade[N];
//criar os circulos
for(i=0;i<N;i++)
{
	//cria as bolas
	bolas[i] = sf::CircleShape(rand()%60 +5);//tamanho
	velocidade[i] = sf::Vector2f(rand()%9+1,rand()%9+1);//velocidade
	bolas[i].setPosition(bolas[i].getRadius()*i*1.6, 60*i);//posição inicial
	bolas[i].setFillColor(sf::Color(rand()%255,rand()%255,rand()%255));//cor
}


while(window.isOpen())
{
	

        window.clear(sf::Color::Black);
        while(window.pollEvent(events))
        {
               	if(events.type == sf::Event::Closed)
		{
         		window.close();
		}

	}
	for(i=0;i<N;i++)
	{
		bolas[i].move(velocidade[i]);
		boulder[i] = bolas[i].getGlobalBounds();
	}
	for(i=0;i<N;i++)
	{
		//for aninhado para verificar colisões entre todas as bolas de maneira constante;
		for(j=i+1;j<N;j++){
			//pega ponto central da bola, visto que sfml trata todas as formas como um quadrado
			centerix = bolas[i].getPosition().x + (boulder[i].width/2);
			centeriy = bolas[i].getPosition().y + (boulder[i].height/2);
			centerjx = bolas[j].getPosition().x + (boulder[j].width/2);
			centerjy = bolas[j].getPosition().y + (boulder[j].height/2);
			dx = (centerix - centerjx);
			dy = (centeriy - centerjy);
			//distancia entre os pontos centrai das bolas			
			distance = std::sqrt((dx*dx) + (dy*dy));
			//pega raio
			r1= bolas[i].getRadius();
			r2= bolas[j].getRadius();
		
			if(distance <= (r1+r2))
			{
				unx = dx/distance;
				uny = dy/distance;
				utx = -uny;
				uty = unx;
				//projetando a velocidade nos vetores(e pegando o escalar)
				vn1 = (unx*velocidade[i].x)+(uny*velocidade[i].y);
				vt1 = (utx*velocidade[i].x)+(uty*velocidade[i].y);
				vn2 = (unx*velocidade[j].x)+(uny*velocidade[j].y);
				vt2 = (utx*velocidade[j].x)+(uty*velocidade[j].y);
				//pega a nova velocidade em relação ao vetor normal
				//usando a medida do raio como massa,
				vnf1 = ((vn1*(r1-r2)+2*(vn2*r2))/(r1+r2))*e;
				vnf2 = ((vn2*(r2-r1)+2*(vn1*r1))/(r1+r2))*e;
				//Atualiza as velocidades da direção normal
				velocidade[i].x = (vnf1 * unx) + (vt1 * utx);
				velocidade[i].y = (vnf1 * uny) + (vt1 * uty);
				velocidade[j].x = (vnf2 * unx) + (vt2 * utx);
				velocidade[j].y = (vnf2 * uny) + (vt2 * uty);
				
				//Não sobrepoem as bolas
				over = r1+r2-distance;
				bolas[i].setPosition(bolas[i].getPosition().x+over*(unx/4),bolas[i].getPosition().y+over*(uny/4));
				bolas[j].setPosition(bolas[j].getPosition().x-(over*(unx/4)),bolas[j].getPosition().y-(over*(uny/4)));
			 
			}
			
		}
	}
	for(i=0;i<N;i++)
	{
		
		if((bolas[i].getPosition().y > (window_size.y - bolas[i].getRadius()*2)) || (bolas[i].getPosition().y<0))
			velocidade[i].y = -velocidade[i].y;
		
		if((bolas[i].getPosition().x > (window_size.x - bolas[i].getRadius()*2)) || (bolas[i].getPosition().x<0))
			velocidade[i].x = -velocidade[i].x;
				
	}
	for(i=0;i<N;i++)
		window.draw(bolas[i]);
	
	
	window.display();

			

}
return 0;
}

//sugestão, fazer o codigo usando funções, adicionar malloc e um botão para escolher bolas, adicionar prevensão mais robusta para não atravessar parede, adicionar setas que indicam velocidade e momento linear nas bolas.

