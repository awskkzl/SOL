/*********************************************************************************
*     File Name           :     model.h
*     Created By          :     yuewu
*     Creation Date       :     [2016-02-16 22:47]
*     Last Modified       :     [2016-03-09 19:21]
*     Description         :     base class for model
**********************************************************************************/

#ifndef LSOL_MODEL_MODEL_H__
#define LSOL_MODEL_MODEL_H__

#include <map>
#include <string>
#include <sstream>
#include <fstream>

#include <json/json.h>

#include <lsol/util/types.h>
#include <lsol/util/reflector.h>
#include <lsol/util/error_code.h>
#include <lsol/loss/loss.h>
#include <lsol/pario/data_point.h>

namespace lsol {
namespace model {

class LSOL_EXPORTS Model {
  DeclareReflectorBase(Model, int class_num);

 public:
  /// \brief  Create a new model
  ///
  /// \param class_num number of classes in the model
  Model(int class_num, const std::string &type);

  virtual ~Model();

  /// \brief  set model parameters
  ///
  /// \param name name of the parameter
  /// \param value value of the parameter in string
  virtual void SetParameter(const std::string &name, const std::string &value);

 public:
  /// \brief  initialize the model for training
  virtual void BeginTrain() {}

  /// \brief  finalize the model after training
  virtual void EndTrain() {}

  /// \brief  predict the label of data
  ///
  /// \param x input data
  /// \param predicts predicted scores on the data
  ///
  /// \return predicted class label
  virtual label_t Predict(const pario::DataPoint &x, float *predicts) = 0;

 public:
  /// \brief  Save model to file
  ///
  /// \param path path to save the model
  ///
  /// \return status code, 0 if saved successfully
  int Save(const std::string &path) const;

  /// \brief  load model from file
  ///
  /// \param path file path of the model
  ///
  /// \return status code 0 if load successfully
  static Model *Load(const std::string &path);

 protected:
  /// \brief  serialize model parameters
  ///
  /// \param root root node to save the parameters
  virtual void GetModelParam(Json::Value &root) const = 0;

  /// \brief  load model parameters from stream
  ///
  /// \param root root node of model info
  ///
  /// \return status code, zero if ok
  virtual int SetModelParam(const Json::Value &root) = 0;

  /// \brief  Get Model Information
  ///
  /// \param root root node of saver
  /// info
  virtual void GetModelInfo(Json::Value &root) const;

  /// \brief  load model from string
  ///
  /// \param root root node of model info
  ///
  /// \return status code, Status_OK if load successfully
  virtual int SetModelInfo(const Json::Value &root);

 protected:
  /// \brief  calibrate labels
  ///
  /// \param x data point
  ///
  /// \return if binary classfication, return 1 or -1, otherwise return the
  /// label as it is
  inline label_t CalibrateLabel(label_t label) {
    return this->clf_num_ == 1 ? (label == 1 ? 1 : -1) : label;
  }

 public:
  int class_num() const { return this->class_num_; }
  int clf_num() const { return this->clf_num_; }
  const std::string &type() const { return this->type_; }
  std::string name() const { return name_; }
  void set_name(const std::string &name) { this->name_ = name; }

  /**
   * PrintModelSettings print the info of optimization algorithm
   */
  std::string model_info() const;

 protected:
  // number of classes
  int class_num_;
  // number of classifiers
  int clf_num_;
  // loss function
  loss::Loss *loss_;

  // online,  batch, etc.
  std::string type_;
  std::string name_;
};

#define RegisterModel(type, name, descr)                                  \
  type *type##_##CreateNewInstance(int class_num) {                       \
    type *ins = new type(class_num);                                      \
    ins->set_name(name);                                                  \
    return ins;                                                           \
  }                                                                       \
  ClassInfo __kClassInfo_##type##__(std::string(name) + "_model",         \
                                    (void *)(type##_##CreateNewInstance), \
                                    descr);

}  // namespace model
}  // namespace lsol

#endif