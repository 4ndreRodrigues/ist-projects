# Import PuLP modeller functions
from pulp import LpProblem, LpMaximize, LpVariable, lpSum, LpStatus,LpBinary, PULP_CBC_CMD, value

def main():

    prob = LpProblem("P3", LpMaximize)

    # Lendo o input
    data = sys.stdin.read().splitlines()
    
    # Primeira linha: número de fábricas, países e crianças
    n, m, t = map(int, data[0].split())

    # Dados das fábricas
    factories = {}
    fcountry = {}
    for i in range(1, n + 1):
        factory_id, country_id, max_stock = map(int, data[i].split())
        factories[factory_id] = max_stock
        fcountry[factory_id] = country_id

    # Dados dos países
    pmax = {}
    pmin = {}
    for i in range(n + 1, n + 1 + m):
        country_id, max_export, min_gifts = map(int, data[i].split())
        pmax[country_id] = max_export
        pmin[country_id] = min_gifts


    variables_per_country = {}
    variables_per_factory = {}
    variables_per_child = {}
    variables_exports_per_country = {}
    vars = []

    # Dados dos pedidos das crianças
    for i in range(n + m + 1, n + 1 + m + t):
        parts = list(map(int, data[i].split()))
        child_id, country_id, *wishes = parts
        for w in wishes:    
            var_name = f"x_{child_id}_{w}"  # Ex: x_1_2 (criança 1 quer brinquedo da fábrica 2)
            new_var = LpVariable(var_name, 0, 1, LpBinary)
            vars.append(new_var)

            if child_id not in variables_per_child:
                variables_per_child[child_id] = []
            variables_per_child[child_id].append(new_var)

            # Relacionar variáveis às fábricas e países
            if w not in variables_per_factory:
                variables_per_factory[w] = []
            variables_per_factory[w].append(new_var)

            if country_id not in variables_per_country:
                variables_per_country[country_id] = []
            variables_per_country[country_id].append(new_var)

            # Relacionar variáveis às exportações por país
            country_factory_id = fcountry[w]
            if country_factory_id not in variables_exports_per_country:
                variables_exports_per_country[country_factory_id] = []
            if country_factory_id != country_id:
                variables_exports_per_country[country_factory_id].append(new_var)


    # Função objetivo: maximizar o número de crianças atendidas
    prob += lpSum(vars), "Maximizar criancas atendidas"

    # Restrições de capacidade das fábricas
    for factory_id, max_stock in factories.items():
        prob += lpSum(variables_per_factory.get(factory_id, [])) <= max_stock, f"Capacidade_Fabrica_{factory_id}"

    # Restrições de exportação máxima por país
    for country_id, max_export in pmax.items():
        prob += lpSum(variables_exports_per_country.get(country_id, [])) <= max_export, f"Max_Export_Pais_{country_id}"

    # Restrições de entrega mínima por país
    for country_id, min_gifts in pmin.items():
        prob += lpSum(variables_per_country.get(country_id, [])) >= min_gifts, f"Min_Entrega_Pais_{country_id}"

    # Restrição: cada criança pode receber no máximo um presente
    for child_id, vars in variables_per_child.items():
        prob += lpSum(vars) <= 1, f"Presente_Unico_Crianca_{child_id}"

    # Resolver problema
    prob.solve(PULP_CBC_CMD(msg = False))

    # Output resultado
    if LpStatus[prob.status] == "Optimal":
        print(int(value(prob.objective)))
    else:
        print(-1)

# Executar a função principal
if __name__ == "__main__":
    import sys
    main()

        