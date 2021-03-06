#ifndef XMLPARSE_H
#define XMLPARSE_H

#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_print.hpp"
#include "types.h"
#include "models.h"
#include "graphs/graph_adj_str.h"

#include <memory>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <QString>
#include <QVector>
#include <QDebug>

using namespace rapidxml;

namespace {
    bool is_number(const std::string &str) {
        try {
            std::stod(str);
        }
        catch (...) {
            return false;
        }
        return true;
    }
}


bool readXmlToModels(const QString& filename,
                     const std::shared_ptr<FeatureModels>& features_model,
                     const std::shared_ptr<TypeModels>& types_model,
                     const std::shared_ptr<DefectModels>& defects_model){
    QVector<Feature> features;
    QVector<Type> types;
    QVector<Defect> defects;

    xml_document<> doc;
    std::ifstream config_file(filename.toStdString(), std::ifstream::in | std::ios::binary);

    //getting size of input file
    config_file.seekg(0, std::ifstream::end);
    int length = config_file.tellg();
    config_file.seekg(0, std::ifstream::beg);
    if (length == 0){
        return false;
    }

    //passing input data to char*
    char *buffer = new char[length + 1];
    config_file.read(buffer, length);
    buffer[length] = '\0';

    //parsing input xml data and setting the root node
    doc.parse<0>(buffer);
    xml_node<> *root_node = doc.first_node("System");

    if (root_node == nullptr ||
            root_node->first_node("FeatureList") == nullptr ||
            root_node->first_node("TypeList") == nullptr ||
            root_node->first_node("DefectList") == nullptr)
    {
        return false;
    }

    //reading all 'Feature' nodes
    xml_node<> *feature_list_node = root_node->first_node("FeatureList");
    for (xml_node<> *feature_node = feature_list_node->first_node("Feature");
         feature_node; feature_node = feature_node->next_sibling()) {
        Feature feature;

        if(!is_number(feature_node->first_node("ComponentsRequired")->value())){
            return false;
        }

        feature.components_required = std::stoi(feature_node->first_node("ComponentsRequired")->value());

        if(!is_number(feature_node->first_node("Reward")->value())){
            return false;
        }

        feature.reward = std::stoi(feature_node->first_node("Reward")->value());

        features << feature;
    }



    //reading all 'Type' nodes
    //GraphAdjStr<int, int> graph(true);
    xml_node<> *type_list_node = root_node->first_node("TypeList");
    for (xml_node<> *type_node = type_list_node->first_node("Type");
         type_node; type_node = type_node->next_sibling()) {
        Type component_type;

        component_type.name = type_node->first_node("Name")->value();

        if(!is_number(type_node->first_node("Cost")->value())){
            return false;
        }

        component_type.cost = std::stoi(type_node->first_node("Cost")->value());

        xml_node<> *feature_id_list_node = type_node->first_node("FeatureIdList");
        for (xml_node<> *feature_id_node = feature_id_list_node->first_node("FeatureId");
             feature_id_node; feature_id_node = feature_id_node->next_sibling()) {
            int new_feature(0);

            if(!is_number(feature_id_node->value())){
                return false;
            }
            new_feature = std::stoi(feature_id_node->value());

            if(!(new_feature < features.size() && new_feature >= 0)){
                return false;
            }

            for(auto &feature:component_type.features){
                if (feature == new_feature){
                    return false;
                }
            }

            component_type.features << new_feature;
        }

        xml_node<> *subcomponent_list_node = type_node->first_node("SubComponentList");
        for (xml_node<> *subcomponent_node = subcomponent_list_node->first_node("SubComponent");
             subcomponent_node; subcomponent_node = subcomponent_node->next_sibling()) {
            int new_sub_component(0);

            if(!is_number(subcomponent_node->value())){
                return false;
            }
            new_sub_component = std::stoi(subcomponent_node->value());

            for(auto &e:component_type.sub_components){
                if (e == new_sub_component){
                    return false;
                }
            }

            if(!(new_sub_component >= 0)){
                return false;
            }
            component_type.sub_components << new_sub_component;
        }

        //graph.add_node(types.size());
        types << component_type;
    }

    //additional check of data correctness for config.component_types[]->sub_components;
    for (int j = 0; j < types.size(); j++) {
        for (int i = 0; i < types[j].sub_components.size(); i++) {
            //graph.add_edge(j, i, 0);
            if(!(types[j].sub_components[i] != j)){
                return false;
            }
            if(!(types[j].sub_components[i] < types.size())){
                return false;
            }
        }
        //if (graph.cycle_exist()) return false;
    }

    //reading all 'Defect' nodes
    xml_node<> *defect_list_node = root_node->first_node("DefectList");
    for (xml_node<> *defect_node = defect_list_node->first_node("Defect");
         defect_node; defect_node = defect_node->next_sibling()) {
        Defect defect;

        if(!is_number(defect_node->first_node("DetectChance")->value())){
            return false;
        }

        defect.detect_chance = std::stod(defect_node->first_node("DetectChance")->value());

        if(defect.detect_chance > 1 || defect.detect_chance <= 0){
            return false;
        }

        xml_node<> *component_type_list_node = defect_node->first_node("ComponentTypeList");
        for (xml_node<> *component_type_node = component_type_list_node->first_node("ComponentType");
             component_type_node; component_type_node = component_type_node->next_sibling()) {
            QPair<int, QPair<int, int>> component_type;

            if(!is_number(component_type_node->first_node("Id")->value())){
                return false;
            }

            component_type.first = std::stoi(component_type_node->first_node("Id")->value());

            if (component_type.first >= types.size() && component_type.first < 0){
                return false;
            }

            if(!is_number(component_type_node->first_node("RepairTime")->value())){
                return false;
            }

            component_type.second.first = std::stoi(component_type_node->first_node("RepairTime")->value());

            if(component_type.second.first < 0){
                return false;
            }

            if(!is_number(component_type_node->first_node("RepairCost")->value())){
                return false;
            }

            component_type.second.second = std::stoi(component_type_node->first_node("RepairCost")->value());

            if(component_type.second.second < 0){
                return false;
            }

            defect.component_types << component_type;
        }

        defects << defect;
    }


    features_model->clearModel();
    types_model->clearModel();
    defects_model->clearModel();

    foreach (auto &feature, features) {
        features_model->addObject(QVariant::fromValue<Feature>(feature));
    }

    foreach (auto &type, types) {
        types_model->addObject(QVariant::fromValue<Type>(type));
    }

    foreach (auto &defect, defects) {
        defects_model->addObject(QVariant::fromValue<Defect>(defect));
    }

    return true;
}

xml_document<>* modelsToXml(std::shared_ptr<FeatureModels> features_model,
                            std::shared_ptr<TypeModels> types_model,
                            std::shared_ptr<DefectModels> defects_model){
    char* str;
    xml_document<> *doc = new xml_document<>;

    xml_node<> *root_node = doc->allocate_node(node_element, "System");
    doc->append_node(root_node);

    //writing all 'Feature' nodes
    xml_node<> *feature_list_node = doc->allocate_node(node_element, "FeatureList");
    root_node->append_node(feature_list_node);


    for (int i = 0; i < features_model->rowCount(); i++) {
        Feature feature = features_model->getObject(i).value<Feature>();

        xml_node<> *feature_node = doc->allocate_node(node_element, "Feature");

        str = doc->allocate_string(QString::number(feature.components_required).toStdString().c_str());
        xml_node<> *components_required_node = doc->allocate_node(node_element, "ComponentsRequired", str);
        feature_node->append_node(components_required_node);

        str = doc->allocate_string(QString::number(feature.reward).toStdString().c_str());
        xml_node<> *reward_node = doc->allocate_node(node_element, "Reward", str);
        feature_node->append_node(reward_node);

        feature_list_node->append_node(feature_node);
    }


    //writing all 'Type' nodes
    xml_node<> *type_list_node = doc->allocate_node(node_element, "TypeList");
    root_node->append_node(type_list_node);

    for (int i = 0; i < types_model->rowCount(); i++) {
        Type type = types_model->getObject(i).value<Type>();

        xml_node<> *type_node = doc->allocate_node(node_element, "Type");

        str = doc->allocate_string(type.name.toStdString().c_str());
        xml_node<> *name_node = doc->allocate_node(node_element, "Name", str);
        type_node->append_node(name_node);

        str = doc->allocate_string(QString::number(type.cost).toStdString().c_str());
        xml_node<> *cost_node = doc->allocate_node(node_element, "Cost", str);
        type_node->append_node(cost_node);

        xml_node<> *type_feature_list_node = doc->allocate_node(node_element, "FeatureIdList");
        for(auto &feature:type.features){
            str = doc->allocate_string(QString::number(feature).toStdString().c_str());
            xml_node<> *feature_node = doc->allocate_node(node_element, "FeatureId", str);
            type_feature_list_node->append_node(feature_node);
        }
        type_node->append_node(type_feature_list_node);

        xml_node<> *type_subcomponent_list_node = doc->allocate_node(node_element, "SubComponentList");
        for(auto &sub_component:type.sub_components){
            auto str = doc->allocate_string(QString::number(sub_component).toStdString().c_str());
            xml_node<> *subcomponent_node = doc->allocate_node(node_element, "SubComponent", str);
            type_subcomponent_list_node->append_node(subcomponent_node);
        }
        type_node->append_node(type_subcomponent_list_node);

        type_list_node->append_node(type_node);
    }

    //writing all 'Defect' nodes
    xml_node<> *defect_list_node = doc->allocate_node(node_element, "DefectList");
    root_node->append_node(defect_list_node);

    for (int i = 0; i < defects_model->rowCount(); i++) {
        Defect defect = defects_model->getObject(i).value<Defect>();

        xml_node<> *defect_node = doc->allocate_node(node_element, "Defect");

        str = doc->allocate_string(QString::number(defect.detect_chance).toStdString().c_str());
        xml_node<> *detect_chance_node = doc->allocate_node(node_element, "DetectChance", str);
        defect_node->append_node(detect_chance_node);

        xml_node<> *defect_ctypes_list_node = doc->allocate_node(node_element, "ComponentTypeList");
        for(auto &ctype:defect.component_types){
            str = doc->allocate_string(QString::number(ctype.first).toStdString().c_str());
            xml_node<> *id_node = doc->allocate_node(node_element, "Id", str);
            defect_ctypes_list_node->append_node(id_node);

            str = doc->allocate_string(QString::number(ctype.second.first).toStdString().c_str());
            xml_node<> *repair_time_node = doc->allocate_node(node_element, "RepairTime", str);
            defect_ctypes_list_node->append_node(repair_time_node);

            str = doc->allocate_string(QString::number(ctype.second.second).toStdString().c_str());
            xml_node<> *repair_cost_node = doc->allocate_node(node_element, "RepairCost", str);
            defect_ctypes_list_node->append_node(repair_cost_node);
        }
        defect_node->append_node(defect_ctypes_list_node);

        defect_list_node->append_node(defect_node);
    }

    return doc;
}


void writeModelsToXml(QString filename, std::shared_ptr<FeatureModels> features_model,
                      std::shared_ptr<TypeModels> types_model,
                      std::shared_ptr<DefectModels> defects_model){
    xml_document<>* doc = modelsToXml(features_model, types_model, defects_model);
    std::ofstream config_file(filename.toStdString(), std::ifstream::out | std::ios::binary);

    config_file << *doc;
}

#endif // XMLPARSE_H
